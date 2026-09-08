"""The spelling search: a body whose structure is right (the lifter's calls, layouts and loops
come from retail) and whose spelling is wrong is closed by search, not by hand.

Every single-step rewrite the lab, the regalloc search and a few families here can make is
a candidate; a beam search keeps the best few texts at every level and rewrites them again.
Candidates are compiled in batches and scored by masked machine words with an alignment
(a fix that shifts every later instruction still counts as progress); a candidate with no
difference left is confirmed by the retail oracle before it is reported. Cost is a few
milliseconds a candidate, so a body gets hundreds of them in seconds.
"""

from __future__ import annotations

import difflib
import json
import re
import time
from concurrent.futures import ThreadPoolExecutor
from typing import Dict, List, Optional, Tuple

from . import lab, oracle, regalloc
from .project import STATE_DIR, Project

# which rewrite families the objdiff row kinds of a diff point at (fixup's targeting)
KIND_FAMILIES = {
    "regalloc": ("decl-order", "inner-scope", "regalloc", "inline-temp", "repeat-to-local", "stmt-swap", "commute", "ptr-local"),
    "ins": ("inline-temp", "inner-scope", "void-return", "return-to-block", "local-type", "hoist-arg", "repeat-to-local"),
    "ins:ext": ("local-type", "param-type", "field-type", "compare-cast"),
    "ins:cmp": ("compare-cast", "local-type", "compare-form"),
    "op": ("local-type", "compare-cast", "field-type", "param-type"),
    "op:cmpw/cmplw": ("compare-cast", "local-type"), "op:cmplw/cmpw": ("compare-cast", "local-type"),
    "op:cmpwi/cmplwi": ("compare-cast", "local-type"), "op:cmplwi/cmpwi": ("compare-cast", "local-type"),
    "reloc": ("near-far", "address-form", "ptr-local"),
    "imm": ("struct-pad", "field-type"),
    "frame": ("decl-order", "inner-scope", "hoist-arg"),
    "schedule": ("stmt-swap", "commute", "inline-temp", "decl-order"),
}
BEAM = 4
LEVELS = 4
MAX_CANDIDATES = 700
FIELD_FLIP = {"u16": "s16", "s16": "u16", "u8": "s8", "s8": "u8", "u32": "s32", "s32": "u32"}


def _fn_span(body: str, name: str):
    return regalloc._function_body_span(body, name)


def extra_families(body: str, name: str) -> List[Tuple[str, str, str]]:
    """(family, label, text): near/far externs, pointer locals for globals, field signedness,
    loop direction, compare casts, call-result temps."""
    out: List[Tuple[str, str, str]] = []
    span = _fn_span(body, name)
    if not span:
        return out
    # 1. near <-> far: a scalar extern becomes an unknown-size array (far) or back
    for m in re.finditer(r"^extern ((?:struct \w+|[us](?:8|16|32)|f32|f64)) ([A-Za-z_]\w*);$", body, re.M):
        t, g = m.groups()
        text = body[:m.start()] + f"extern {t} {g}[];" + body[m.end():]
        if t.startswith("struct"):
            text = re.sub(rf"(?<![\w>.]){re.escape(g)}\.unk_", f"{g}[0].unk_", text)
        else:
            text = re.sub(rf"(?<![\w>.&]){re.escape(g)}\b(?!\s*\[|\s*=\s*\()", f"{g}[0]", text)
        out.append(("near-far", f"{g} far", text))
    for m in re.finditer(r"^extern ((?:struct \w+|[us](?:8|16|32)|f32|f64)) ([A-Za-z_]\w*)\[\];$", body, re.M):
        t, g = m.groups()
        text = body[:m.start()] + f"extern {t} {g};" + body[m.end():]
        text = text.replace(f"{g}[0]", g)
        out.append(("near-far", f"{g} near", text))
    # 2. a global struct accessed several times through a pointer local
    inner = body[span[1]:span[2]]
    for m in re.finditer(r"^extern struct (\w+) ([A-Za-z_]\w*)(\[\])?;$", body, re.M):
        st, g, arr = m.groups()
        uses = re.findall(rf"(?<![\w>.]){re.escape(g)}(?:\[0\])?\.unk_", inner)
        if len(uses) < 2 or f"p_{g}" in body:
            continue
        acc = f"{g}[0].unk_" if arr else f"{g}.unk_"
        new_inner = inner.replace(acc, f"p_{g}->unk_")
        # the assignment goes before the first statement; the declaration with the locals
        first_stmt = re.search(r"\n(\s*)(?![A-Za-z_][\w ]*\*?\s*[A-Za-z_]\w*(\[[^\]]*\])*;\n)([^\n]*;)", new_inner)
        if not first_stmt:
            continue
        at = first_stmt.start(3)
        new_inner = new_inner[:at] + f"p_{g} = (struct {st} *)&{g};\n    " + new_inner[at:]
        new_inner = f"\n    struct {st} *p_{g};" + new_inner
        out.append(("ptr-local", f"pointer local for {g}", body[:span[1]] + new_inner + body[span[2]:]))
    # 3. field signedness in block-private structs
    for m in re.finditer(r"^(    )([us](?:8|16|32)) (unk_[0-9A-F]+)(\[[^\]]*\])?;$", body, re.M):
        t = m.group(2)
        out.append(("field-type", f"{m.group(3)}: {t}->{FIELD_FLIP[t]}", body[:m.start(2)] + FIELD_FLIP[t] + body[m.end(2):]))
    # 4. counted loop direction
    for m in re.finditer(r"for \((\w+) = ([^;]+); \1 != 0; \1--\)", body):
        v, n = m.groups()
        out.append(("loop-form", "count down -> up", body[:m.start()] + f"for ({v} = 0; {v} < {n}; {v}++)" + body[m.end():]))
    for m in re.finditer(r"for \((\w+) = 0; \1 < ([^;]+); \1\+\+\)", body):
        v, n = m.groups()
        out.append(("loop-form", "count up -> down", body[:m.start()] + f"for ({v} = {n}; {v} != 0; {v}--)" + body[m.end():]))
    # 5. compare casts: drop or flip the cast on a compare operand
    for m in re.finditer(r"\((s32|u32)\)([A-Za-z_][\w>.\-\[\]]*) (==|!=|<|>|<=|>=)", inner):
        s0 = span[1] + m.start(); e0 = span[1] + m.end()
        other = "u32" if m.group(1) == "s32" else "s32"
        out.append(("compare-cast", f"drop ({m.group(1)}) on {m.group(2)}", body[:s0] + f"{m.group(2)} {m.group(3)}" + body[e0:]))
        out.append(("compare-cast", f"({m.group(1)})->({other}) on {m.group(2)}", body[:s0] + f"({other}){m.group(2)} {m.group(3)}" + body[e0:]))
    # 6. a returned parameter or constant on a function whose r3 is scratch: a void function
    ret = re.search(r"^(\s*)return ((?:arg\d+|v\d+|-?\d+|0x[0-9A-Fa-f]+));\n\}\s*$", body, re.M)
    if ret:
        sig = re.search(rf"^(u32|s32|s16|u16|s8|u8)( {re.escape(name)}\()", body, re.M)
        if sig:
            text = body[:sig.start(1)] + "void" + body[sig.end(1):]
            text = text.replace(ret.group(0), "}\n", 1)
            out.append(("void-return", "drop the returned value", text))
    # 7. an early return is the source's if-block around the tail: `if (c) { return; } S` -> `if (!c) { S }`
    for m in re.finditer(r"^(\s*)if \((.+?)\) \{ return; \}[^\n]*\n", inner, re.M):
        cond = m.group(2)
        tail = inner[m.end():]
        close = tail.rfind("\n")
        stmts_tail = tail[:close] if close >= 0 else tail
        if "{" in stmts_tail or "}" in stmts_tail or "return" in stmts_tail or not stmts_tail.strip():
            continue
        inv = {"==": "!=", "!=": "==", "<": ">=", ">": "<=", "<=": ">", ">=": "<"}
        mm = re.fullmatch(r"(.+?) (==|!=|<|>|<=|>=) (.+)", cond)
        ncond = f"{mm.group(1)} {inv[mm.group(2)]} {mm.group(3)}" if mm else f"!({cond})"
        new_inner = inner[:m.start()] + f"{m.group(1)}if ({ncond}) {{\n" + stmts_tail + f"\n{m.group(1)}}}" + tail[close:]
        out.append(("return-to-block", "early return -> if block", body[:span[1]] + new_inner + body[span[2]:]))
    # 8. a temporary for a call result used once: inline it
    for m in re.finditer(r"^(\s*)(t\d+) = ([A-Za-z_]\w*\([^;]*\));\n", inner, re.M):
        tn, call = m.group(2), m.group(3)
        rest = inner[m.end():]
        if len(re.findall(rf"\b{tn}\b", rest)) == 1:
            new_inner = inner[:m.start()] + re.sub(rf"\b{tn}\b", call, rest, count=1)
            out.append(("inline-call", f"inline {tn}", body[:span[1]] + new_inner + body[span[2]:]))
    return out


def all_rewrites(body: str, name: str) -> List[Tuple[str, str, str]]:
    out: List[Tuple[str, str, str]] = []
    for fn in (lambda b, n: lab.perturbations(b, n),
               lambda b, n: [("regalloc", l, t) for l, t in regalloc.rewrites(b, n)],
               extra_families):
        try:
            out += fn(body, name)
        except Exception:
            continue
    return out


def fitness(tw: List[int], ow: List[int]) -> Tuple[float, float]:
    """(aligned matched words as a percent, positional percent)."""
    if not tw or not ow:
        return (0.0, 0.0)
    sm = difflib.SequenceMatcher(None, tw, ow, autojunk=False)
    matched = sum(b.size for b in sm.get_matching_blocks())
    n = max(len(tw), len(ow))
    pos, _ = oracle.word_score(tw, ow)
    return (100.0 * matched / n, pos)


def search(p: Project, symbol: str, body: str, budget_s: float = 10.0, beam: int = BEAM, levels: int = LEVELS,
           max_candidates: int = MAX_CANDIDATES) -> Dict[str, object]:
    t0 = time.time()
    sym = p.resolve(symbol)
    out: Dict[str, object] = {"matched": False, "body": None, "tried": 0, "base": 0.0, "best": 0.0, "path": [], "secs": 0.0}
    if sym is None:
        return out
    target = p.target_object_for(sym)
    if target is None:
        return out
    tw = oracle.words(target, sym.name)
    if not tw:
        return out
    key = p.key(sym).replace(":", "__")
    root = STATE_DIR / "spell" / key
    root.mkdir(parents=True, exist_ok=True)
    base_src = root / "base.c"; base_src.write_text(body)
    mw, extra = oracle.version_for(p, sym, base_src)
    seen: Dict[str, Tuple[float, float]] = {}

    def evaluate(texts: List[str]) -> List[Optional[Tuple[float, float]]]:
        d = root / "b"
        d.mkdir(exist_ok=True)
        for old in d.glob("*.c"):
            old.unlink()
        srcs = []
        for j, text in enumerate(texts):
            f = d / f"c{j}.c"; f.write_text(text); srcs.append(f)
        objs = oracle.compile_many(p, sym.module, srcs, d / "obj", mw, extra)
        res: List[Optional[Tuple[float, float]]] = []
        for f in srcs:
            o = objs.get(f)
            ow = oracle.words(o, sym.name) if o else None
            res.append(fitness(tw, ow) if ow else None)
        return res

    def confirm(text: str) -> bool:
        w = root / "winner.c"; w.write_text(text)
        r = oracle.check(p, symbol, 0, source=w, mw_version=mw)
        return bool(r.ok and (r.matched or r.matched_pool) and oracle.unit_fully_matches(r) is None)

    base_fit = evaluate([body])[0]
    if base_fit is None:
        out["error"] = "base does not compile"
        return out
    # the objdiff rows of the base, once: their kinds say which families are likely
    weights: Dict[str, float] = {}
    try:
        from . import stuck  # scoped: stuck imports oracle; kept local to this optional seeding
        o0 = root / "b" / "obj" / "c0.o"
        rows = oracle.function_rows(p, sym.name, target, o0) if o0.exists() else None
        if rows:
            kinds = {k for k in stuck.row_kinds(rows[0], rows[1]) if k}
            for k in kinds:
                for fam in KIND_FAMILIES.get(k.split(":")[0], ()):
                    weights[fam] = weights.get(fam, 0) + 1
                for fam in KIND_FAMILIES.get(k, ()):
                    weights[fam] = weights.get(fam, 0) + 2
    except Exception:
        weights = {}
    out["base"] = base_fit[1]
    seen[body] = base_fit
    if base_fit[1] >= 100.0 and confirm(body):
        out.update(matched=True, body=body, best=100.0)
        return out
    frontier: List[Tuple[Tuple[float, float], str, List[str]]] = [(base_fit, body, [])]
    best = (base_fit, body, [])
    tried = 0
    for _level in range(levels):
        if time.time() - t0 > budget_s or tried >= max_candidates:
            break
        cands: List[Tuple[str, List[str]]] = []
        for fit, text, path in frontier:
            for fam, label, t2 in all_rewrites(text, sym.name):
                if t2 in seen or t2 == text:
                    continue
                seen[t2] = (-1.0, -1.0)
                cands.append((t2, path + [f"{fam}: {label}"]))
                if len(cands) >= max_candidates - tried:
                    break
            if len(cands) >= max_candidates - tried:
                break
        if not cands:
            break
        if weights:
            # the families the diff rows point at first: a budget cut keeps the likely ones
            cands.sort(key=lambda c: -weights.get(c[1][-1].split(":")[0], 0))
        res = evaluate([t for t, _ in cands])
        tried += len(cands)
        scored: List[Tuple[Tuple[float, float], str, List[str]]] = []
        for (text, path), r in zip(cands, res):
            if r is None:
                continue
            seen[text] = r
            scored.append((r, text, path))
            if r[1] >= 100.0 and confirm(text):
                out.update(matched=True, body=text, best=100.0, tried=tried, path=path, secs=round(time.time() - t0, 2))
                return out
        scored.sort(key=lambda x: (-x[0][0], -x[0][1]))
        if scored and scored[0][0] > best[0]:
            best = scored[0]
        # the beam: the best few at or above the base (no drifting down)
        frontier = [x for x in scored[:beam] if x[0] >= base_fit]
        if not frontier:
            break
    out.update(tried=tried, best=best[0][1], body=best[1] if best[1] != body else None, path=best[2],
               aligned=best[0][0], secs=round(time.time() - t0, 2))
    if best[1] != body:
        (root / "best.c").write_text(best[1])  # the next round starts here
    return out


def run_drafts(p: Project, min_pct: float = 0.0, max_pct: float = 100.0, limit: int = 5000, workers: int = 3,
               budget_s: float = 10.0, submit: bool = True, only: Optional[List[str]] = None) -> Dict[str, object]:
    """The search over the lifter's current drafts (.fzgx/draftscan): every draft that compiles
    and scores in [min_pct, max_pct). A draft the last round improved starts from that body.
    Matches are submitted as `spell`."""
    d = STATE_DIR / "draftscan"
    scores = json.loads((d / "scores.json").read_text())
    items = []
    for s, m, size, pct, k in scores:
        if pct < 0 or pct >= 100 or pct < min_pct or pct >= max_pct:
            continue
        if only and s not in only:
            continue
        sym = p.resolve(s)
        if sym is None or p.unit_of(sym):
            continue
        f = d / f"{m}__{s}.c"
        best = STATE_DIR / "spell" / p.key(sym).replace(":", "__") / "best.c"
        src = best if best.exists() and best.stat().st_mtime >= f.stat().st_mtime else f
        if f.exists():
            items.append((s, m, size, pct, src.read_text()))
    items.sort(key=lambda x: -x[3])
    return run_bodies(p, items[:limit], workers, budget_s, submit, agent="spell")


def run_attempts(p: Project, min_pct: float = 60.0, limit: int = 5000, workers: int = 3, budget_s: float = 10.0,
                 submit: bool = True, module: Optional[str] = None) -> Dict[str, object]:
    """The search over the agents' saved plateau bodies (the ledger's best attempts)."""
    import sqlite3  # scoped: only this reader touches the ledger directly
    db = sqlite3.connect(str(STATE_DIR / "ledger.db"))
    q = ("select f.symbol, f.module, f.size, a.best_body_path, max(a.best_in_attempt) from functions f join attempts a on a.symbol=f.symbol "
         "where f.status='unmatched' and a.best_body_path is not null and a.best_in_attempt>=?" + (" and f.module=?" if module else "") + " group by f.symbol order by 5 desc")
    rows = db.execute(q, [min_pct] + ([module] if module else [])).fetchall()
    items = []
    from pathlib import Path  # scoped: a single path test
    for s, m, size, path, pct in rows:
        if path and Path(path).exists():
            items.append((s, m, size, pct or 0.0, Path(path).read_text()))
    return run_bodies(p, items[:limit], workers, budget_s, submit, agent="spell")


def run_bodies(p: Project, items, workers: int = 3, budget_s: float = 10.0, submit: bool = True, agent: str = "spell") -> Dict[str, object]:
    from . import api  # scoped: api imports the search modules; importing it at load would be a cycle
    t0 = time.time()

    def one(it):
        s, m, size, pct, text = it
        try:
            r = search(p, s, text, budget_s=budget_s)
        except Exception as e:
            return (s, m, size, pct, {"error": str(e)[:80], "matched": False, "best": pct, "tried": 0})
        return (s, m, size, pct, r)

    with ThreadPoolExecutor(max_workers=workers) as ex:
        results = list(ex.map(one, items))
    matched = []; improved = 0; tried = 0
    families: Dict[str, int] = {}
    for s, m, size, pct, r in results:
        tried += r.get("tried", 0)
        if r.get("matched") and r.get("body"):
            for step in r.get("path", []):
                fam = step.split(":")[0]
                families[fam] = families.get(fam, 0) + 1
            if submit:
                sym = p.resolve(s)
                work = p.work_path(p.key(sym)); work.parent.mkdir(parents=True, exist_ok=True); work.write_text(r["body"])
                sr = api.submit(p, s, agent=agent, message="spelling search: " + " + ".join(r.get("path", []))[:200], harness="fzgx", model="spell")
                if sr.get("ok"):
                    matched.append((s, pct, r.get("path")))
            else:
                matched.append((s, pct, r.get("path")))
        elif r.get("body"):
            improved += 1
    out = {"drafts": len(items), "matched": matched, "improved": improved, "candidates": tried, "families": families,
           "secs": round(time.time() - t0, 1),
           "results": [(s, m, size, pct, {k: v for k, v in r.items() if k != "body"}) for s, m, size, pct, r in results]}
    (STATE_DIR / "spell").mkdir(parents=True, exist_ok=True)
    (STATE_DIR / "spell" / "results.json").write_text(json.dumps(out, indent=1))
    return out
