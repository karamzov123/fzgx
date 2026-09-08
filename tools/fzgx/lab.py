"""The perturbation lab: what source change closes a body that is already at 97%+?

For every near-match (a plateaued attempt or a lifter draft) the lab enumerates
semantics-preserving rewrites of the C, compiles them in one batch, scores each against
retail row by row, and records which rewrite closed the function or repaired rows. Across
the corpus that gives (a) matches, submitted like any other, and (b) a table of which
rewrite family works for which failure mode: the deterministic fixup rules and the
compiler notes for the agents come from that table, not from guesses.
"""

from __future__ import annotations

import itertools
import json
import re
import sqlite3
import time
from pathlib import Path
from typing import Dict, List, Optional, Tuple

from . import fixup, lift, oracle, stuck
from .project import STATE_DIR, Project

DECL_RE = re.compile(r"^\s*(?:const\s+)?((?:struct\s+\w+\s*\*?|[A-Za-z_]\w*(?:\s*\*)?))\s+([A-Za-z_]\w*)((?:\[[^\]]*\])*)\s*;\s*$")
FLIP = {"s32": "u32", "u32": "s32", "s16": "u16", "u16": "s16", "s8": "u8", "u8": "s8", "f32": "f64", "f64": "f32", "int": "u32"}


def _function_body_span(body: str, name: str) -> Optional[Tuple[int, int, int]]:
    """(start of the function, index after '{', index of the closing '}')."""
    m = re.search(rf"\b{re.escape(name)}\s*\([^;{{]*\)\s*\{{", body)
    if not m:
        return None
    depth, i = 0, m.end() - 1
    while i < len(body):
        if body[i] == "{":
            depth += 1
        elif body[i] == "}":
            depth -= 1
            if depth == 0:
                return m.start(), m.end(), i
        i += 1
    return None


def _locals(body: str, span) -> List[Tuple[int, int, str, str, str]]:
    """Leading local declarations of the function: (line start, line end, type, name, dims)."""
    out = []
    pos = span[1]
    for m in re.finditer(r"[^\n]*\n", body[span[1]:span[2]]):
        line = m.group(0)
        s0 = span[1] + m.start()
        if not line.strip():
            continue
        dm = DECL_RE.match(line.rstrip("\n"))
        if not dm:
            break
        out.append((s0, s0 + len(line), dm.group(1), dm.group(2), dm.group(3)))
    return out


def perturbations(body: str, name: str) -> List[Tuple[str, str, str]]:
    """(family, label, text) for every single rewrite."""
    out: List[Tuple[str, str, str]] = []
    span = _function_body_span(body, name)
    if not span:
        return out
    locs = _locals(body, span)
    # 1. declaration order: every permutation of up to 5 locals, else reverse and adjacent swaps
    if len(locs) >= 2:
        lines = [body[s:e] for s, e, *_ in locs]
        blk_s, blk_e = locs[0][0], locs[-1][1]
        perms = list(itertools.permutations(range(len(lines)))) if len(lines) <= 5 else \
            [tuple(reversed(range(len(lines))))] + [tuple(list(range(i)) + [i + 1, i] + list(range(i + 2, len(lines)))) for i in range(len(lines) - 1)]
        for perm in perms:
            if list(perm) == list(range(len(lines))):
                continue
            out.append(("decl-order", "order " + ",".join(locs[i][3] for i in perm), body[:blk_s] + "".join(lines[i] for i in perm) + body[blk_e:]))
    # 2. local types
    for s, e, typ, nm, dims in locs:
        t0 = typ.strip()
        if t0 in FLIP:
            out.append(("local-type", f"{nm}: {t0}->{FLIP[t0]}", body[:s] + body[s:e].replace(t0, FLIP[t0], 1) + body[e:]))
    # 3. inner block scope for a local: declare it where it is first used
    inner = body[span[1]:span[2]]
    for s, e, typ, nm, dims in locs:
        uses = [m.start() for m in re.finditer(rf"\b{re.escape(nm)}\b", inner)]
        uses = [u for u in uses if span[1] + u >= locs[-1][1]]
        if not uses:
            continue
        first = span[1] + uses[0]
        line_start = body.rfind("\n", 0, first) + 1
        last = span[1] + uses[-1]
        line_end = body.find("\n", last) + 1
        decl = body[s:e]
        text = body[:s] + body[e:]
        shift = e - s
        a, b = line_start - shift, line_end - shift
        text = text[:a] + "{\n" + decl + text[a:b] + "}\n" + text[b:]
        out.append(("inner-scope", f"scope {nm}", text))
    # 4. temp inlining: `v = expr;` used once afterwards -> substitute
    for m in re.finditer(r"^(\s*)([A-Za-z_]\w*) = ([^;]+);\n", body[span[1]:span[2]], re.M):
        v, expr = m.group(2), m.group(3)
        rest = body[span[1] + m.end():span[2]]
        uses = list(re.finditer(rf"\b{re.escape(v)}\b", rest))
        if len(uses) == 1 and not re.search(rf"\b{re.escape(v)}\s*=", rest):
            new_rest = rest[:uses[0].start()] + f"({expr})" + rest[uses[0].end():]
            out.append(("inline-temp", f"inline {v}", body[:span[1] + m.start()] + body[span[1] + m.end():span[1] + m.end()] + new_rest.join(["", ""]) if False else body[:span[1] + m.start()] + new_rest + body[span[2]:]))
    # 5. hoist a call argument into a temp right before the call
    for m in re.finditer(r"^(\s*)((?:[A-Za-z_][\w>.\-\[\]]* = )?)([A-Za-z_]\w*)\(([^;]*)\);\n", body[span[1]:span[2]], re.M):
        indent, lhs, callee, args = m.groups()
        parts = [a.strip() for a in re.split(r",(?![^()]*\))", args)] if args.strip() else []
        for k, a in enumerate(parts):
            if re.fullmatch(r"[A-Za-z_]\w*|-?\d+|0x[0-9A-Fa-f]+|&[A-Za-z_]\w*|\d+\.\d*f?", a):
                continue
            tname = f"lab_t{k}"
            newparts = list(parts); newparts[k] = tname
            stmt = f"{indent}u32 {tname};\n" if False else ""
            text = body[:span[1] + m.start()] + f"{indent}{tname} = {a};\n{indent}{lhs}{callee}({', '.join(newparts)});\n" + body[span[1] + m.end():]
            # declare the temp with the locals (type guessed from the expression: float if it has a float field or literal)
            ty = "f32" if re.search(r"\d\.\d|f32|unk_\w*f\b", a) else "u32"
            ins_at = locs[-1][1] if locs else span[1] + 1
            text = text[:ins_at] + f"    {ty} {tname};\n" + text[ins_at:]
            out.append(("hoist-arg", f"hoist arg {k} of {callee}", text))
    # 6. increment forms
    for m in re.finditer(r"^(\s*)(\S[^=\n]*?) = \2 \+ 1;\n", body, re.M):
        out.append(("increment", "x = x + 1 -> x++", body[:m.start()] + f"{m.group(1)}{m.group(2)}++;\n" + body[m.end():]))
    for m in re.finditer(r"^(\s*)(\S[^\n]*?)\+\+;\n", body, re.M):
        out.append(("increment", "x++ -> x = x + 1", body[:m.start()] + f"{m.group(1)}{m.group(2)} = {m.group(2)} + 1;\n" + body[m.end():]))
    # 7. return forms
    for m in re.finditer(r"^(\s*)return ([A-Za-z_]\w*\([^;]*\));\n", body, re.M):
        out.append(("return-form", "return f() -> f(); return", body[:m.start()] + f"{m.group(1)}{m.group(2)};\n{m.group(1)}return;\n" + body[m.end():]))
    # 8. address forms
    for m in re.finditer(r"\(struct (\w+) \*\)&([A-Za-z_]\w*)", body):
        out.append(("address-form", f"drop cast on &{m.group(2)}", body[:m.start()] + f"&{m.group(2)}" + body[m.end():]))
    # 9. compare forms
    for m in re.finditer(r"\(([^()]+?) != 0\)", body):
        out.append(("compare-form", "x != 0 -> x", body[:m.start()] + f"({m.group(1)})" + body[m.end():]))
    for m in re.finditer(r"\(([^()]+?) == 0\)", body):
        out.append(("compare-form", "x == 0 -> !x", body[:m.start()] + f"(!({m.group(1)}))" + body[m.end():]))
    # 11. commute the operands of a commutative binary operator (register assignment follows
    #     operand order)
    for m in re.finditer(r"\(([A-Za-z_][\w>.\-\[\]]*) ([+*&|^]|==|!=) ([A-Za-z_][\w>.\-\[\]]*|-?\d+|0x[0-9A-Fa-f]+)\)", body[span[1]:span[2]]):
        a_, op, b_ = m.groups()
        if a_ == b_:
            continue
        s0 = span[1] + m.start(); e0 = span[1] + m.end()
        out.append(("commute", f"{a_} {op} {b_} -> {b_} {op} {a_}", body[:s0] + f"({b_} {op} {a_})" + body[e0:]))
    # 12. swap two adjacent statements that share no identifier (independent: order is free)
    stmts_ = [(m.start() + span[1], m.end() + span[1], m.group(0)) for m in re.finditer(r"^[ \t]*[^\n{}]+;\n", body[span[1]:span[2]], re.M)]
    for (s1, e1, t1), (s2, e2, t2) in zip(stmts_, stmts_[1:]):
        if e1 != s2:
            continue
        ids1 = set(re.findall(r"[A-Za-z_]\w*", t1)); ids2 = set(re.findall(r"[A-Za-z_]\w*", t2))
        if ids1 & ids2 or "(" in t1 and "(" in t2:  # shared names or two calls: order carries meaning
            continue
        out.append(("stmt-swap", f"swap `{t1.strip()[:30]}` / `{t2.strip()[:30]}`", body[:s1] + t2 + t1 + body[e2:]))
    # 13. struct layout: shift every field of a block-private struct by a small delta (front padding)
    for m in re.finditer(r"(?:typedef\s+)?struct\s+\w*\s*\{([^}]*)\}", body):
        inner = m.group(1)
        for delta in (4, 8, -4, -8):
            if delta > 0:
                out.append(("struct-pad", f"+{delta} front padding", body[:m.start(1)] + f"\n    u8 lab_pad[{delta}];" + inner + body[m.end(1):]))
            else:
                pm = re.match(r"\s*u8\s+(\w+)\[(0x[0-9A-Fa-f]+|\d+)\];", inner)
                if pm and int(pm.group(2), 0) + delta > 0:
                    out.append(("struct-pad", f"{delta} front padding", body[:m.start(1)] + re.sub(r"^\s*u8\s+\w+\[[^\]]+\];", f"\n    u8 {pm.group(1)}[{int(pm.group(2), 0) + delta}];", inner, count=1) + body[m.end(1):]))
    # 14. a repeated non-trivial expression becomes a local computed once
    exprs = {}
    for m in re.finditer(r"[A-Za-z_]\w*(?:->|\.)\w+(?:(?:->|\.)\w+)*|[A-Za-z_]\w*\[[^\]]+\]", body[span[1]:span[2]]):
        exprs[m.group(0)] = exprs.get(m.group(0), 0) + 1
    for ex, n in exprs.items():
        if n < 2 or re.search(rf"{re.escape(ex)}\s*=[^=]", body[span[1]:span[2]]):
            continue
        first = body.find(ex, span[1])
        line_start = body.rfind("\n", 0, first) + 1
        ins_at = locs[-1][1] if locs else span[1] + 1
        ty = "f32" if re.search(r"f32|\bf\w*\b", ex) and False else "u32"
        text = body[:line_start] + f"    lab_v = {ex};\n" + body[line_start:]
        text = text.replace(ex, "lab_v")
        text = text.replace("    lab_v = lab_v;\n", f"    lab_v = {ex};\n", 1)
        text = text[:ins_at] + f"    {ty} lab_v;\n" + text[ins_at:]
        out.append(("repeat-to-local", f"local for {ex}", text))
    # 10. parameters: signedness flips and unused trailing parameters
    pm = re.search(rf"\b{re.escape(name)}\s*\(([^)]*)\)\s*\{{", body)
    if pm:
        params = [x.strip() for x in pm.group(1).split(",")] if pm.group(1).strip() not in ("", "void") else []
        for k, prm in enumerate(params):
            t0 = prm.split()[0]
            if t0 in FLIP:
                np = list(params); np[k] = prm.replace(t0, FLIP[t0], 1)
                out.append(("param-type", f"param {k}: {t0}->{FLIP[t0]}", body[:pm.start(1)] + ", ".join(np) + body[pm.end(1):]))
        for extra in (1, 2, 3):
            np = params + [f"u32 lab_unused{i}" for i in range(extra)]
            out.append(("param-count", f"+{extra} unused params", body[:pm.start(1)] + ", ".join(np) + body[pm.end(1):]))
    return out


def corpus(p: Project, min_pct: float = 97.0, limit: int = 400) -> List[Tuple[str, str, float, str]]:
    """Near matches: plateaued attempts and lifter drafts at min_pct or better."""
    db = sqlite3.connect(str(STATE_DIR / "ledger.db"))
    out = []
    rows = db.execute("select f.symbol, f.module, a.best_body_path, a.best_in_attempt from functions f join attempts a on a.symbol=f.symbol "
                      "where f.status='unmatched' and a.best_body_path is not null and a.best_in_attempt>=? order by a.best_in_attempt desc", (min_pct,)).fetchall()
    seen = set()
    for s, m, path, pct in rows:
        if s in seen or not Path(path).exists():
            continue
        seen.add(s); out.append((s, Path(path).read_text(), pct, "attempt"))
    # lifter drafts: the scores the last `fzgx trivial` pass recorded (no rescoring here)
    scores_path = STATE_DIR / "lift" / "scores.json"
    if scores_path.exists():
        try:
            for s, rec in json.loads(scores_path.read_text()).items():
                if s not in seen and not rec.get("matched") and (rec.get("percent") or 0) >= min_pct:
                    seen.add(s); out.append((s, rec["text"], rec["percent"], "draft"))
        except ValueError:
            pass
    return out[:limit]


def run(p: Project, min_pct: float = 97.0, limit: int = 400, budget_s: float = 60.0, submit: bool = True) -> Dict[str, object]:
    t0 = time.time()
    items = corpus(p, min_pct, limit)
    results = []
    closes: Dict[str, int] = {}; improves: Dict[str, int] = {}; tries: Dict[str, int] = {}
    matched = []
    from concurrent.futures import ThreadPoolExecutor
    def one(item):
        s, body, pct, kind = item
        sym = p.resolve(s)
        if sym is None:
            return None
        tgt = p.target_object_for(sym)
        if tgt is None:
            return None
        pert = perturbations(body, sym.name)
        if not pert:
            return None
        bdir = STATE_DIR / "lab" / s.replace(":", "__"); bdir.mkdir(parents=True, exist_ok=True)
        for old in bdir.glob("*.c"):
            old.unlink()
        srcs = []
        for i, (fam, label, text) in enumerate(pert):
            f = bdir / f"c{i}.c"; f.write_text(text); srcs.append(f)
        base_src = bdir / "base.c"; base_src.write_text(body)
        objs = oracle.compile_many(p, sym.module, [base_src] + srcs, bdir / "obj")
        base_rows = oracle.function_rows(p, sym.name, tgt, objs[base_src]) if objs.get(base_src) else None
        if not base_rows:
            return None
        def taddr(r):
            return (r.get("instruction") or {}).get("address")
        bk = stuck.row_kinds(base_rows[0], base_rows[1])
        base_diff = {taddr(l) for l, k in zip(base_rows[0], bk) if k and taddr(l) is not None}
        per_fn = {"symbol": s, "kind": kind, "base": base_rows[2], "closed_by": [], "improved_by": [], "tries": {}, "best_text": None}
        best_text = None
        for i, (fam, label, text) in enumerate(pert):
            per_fn["tries"][fam] = per_fn["tries"].get(fam, 0) + 1
            o = objs.get(srcs[i])
            if not o:
                continue
            rows = oracle.function_rows(p, sym.name, tgt, o)
            if not rows:
                continue
            k2 = stuck.row_kinds(rows[0], rows[1])
            now = {taddr(l) for l, k in zip(rows[0], k2) if k and taddr(l) is not None}
            if rows[2] >= 100.0:
                per_fn["closed_by"].append((fam, label))
                if best_text is None:
                    best_text = text
            elif rows[2] > base_rows[2] + 0.05 and not (now - base_diff):
                per_fn["improved_by"].append((fam, label, round(rows[2], 1)))
        per_fn["best_text"] = best_text
        return per_fn
    with ThreadPoolExecutor(max_workers=12) as ex:
        results = [r for r in ex.map(one, items) if r]
    for r in results:
        for fam, n in r["tries"].items():
            tries[fam] = tries.get(fam, 0) + n
        for fam, _ in r["closed_by"]:
            closes[fam] = closes.get(fam, 0) + 1
        for fam, _, _ in r["improved_by"]:
            improves[fam] = improves.get(fam, 0) + 1
    # submissions are serial (carves take locks)
    if submit:
        from . import api
        for r in results:
            if not r["best_text"]:
                continue
            s = r["symbol"]; sym = p.resolve(s)
            wdir = STATE_DIR / "lab" / s.replace(":", "__"); (wdir / "winner.c").write_text(r["best_text"])
            chk = oracle.check(p, s, 0, source=wdir / "winner.c")
            if chk.ok and (chk.matched or chk.matched_pool) and oracle.unit_fully_matches(chk) is None:
                work = p.work_path(s if ":" in s else p.key(sym)); work.parent.mkdir(parents=True, exist_ok=True); work.write_text(r["best_text"])
                sr = api.submit(p, s, agent="lab", message=f"perturbation lab: {r['closed_by'][0][1]}", harness="fzgx", model="lab")
                if sr.get("ok"):
                    matched.append((s, r["closed_by"][0]))
    for r in results:
        r.pop("best_text", None)
    out = {"corpus": len(items), "functions": len(results), "matched": matched, "closes": closes, "improves": improves, "tries": tries,
           "results": results, "secs": round(time.time() - t0, 1)}
    (STATE_DIR / "lab.json").write_text(json.dumps(out, indent=1))
    return out


def summary(out: Dict[str, object]) -> str:
    lines = [f"corpus {out['corpus']} near matches, {out['functions']} perturbed, {len(out['matched'])} matched in {out['secs']}s", "",
             "| family | tries | improved | closed |", "|---|---|---|---|"]
    for fam in sorted(out["tries"], key=lambda f: (-out["closes"].get(f, 0), -out["improves"].get(f, 0))):
        lines.append(f"| {fam} | {out['tries'][fam]} | {out['improves'].get(fam, 0)} | {out['closes'].get(fam, 0)} |")
    lines.append("")
    for r in out["results"]:
        if r["closed_by"]:
            lines.append(f"- {r['symbol']} ({r['kind']}, {r['base']:.1f}%): closed by " + "; ".join(f"{f}: {l}" for f, l in r["closed_by"][:3]))
    return "\n".join(lines)
