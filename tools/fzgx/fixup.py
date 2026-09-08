"""Deterministic last-resort repairs of a plateaued body, cheap enough to run inside an
agent's session before it releases (seconds, not minutes).

The object diff says what kind of difference is left (stuck.classify_rows); each kind has a
small, enumerable search over the source that the oracle verifies:

  signedness / sign-extension rows   flip the signedness or width of one integer declaration
  (cmpw/cmplw, extsh, extsb, clrlwi,   at a time (params, locals, block-private struct fields)
   wrong int-to-double constant)

Every candidate is one compile plus one objdiff (~0.4 s). The first body that matches wins;
otherwise the best percentage seen is reported, never applied.
"""

from __future__ import annotations

import re
import time
from pathlib import Path
from typing import Dict, List, Optional, Tuple

from . import oracle, stuck
from .project import STATE_DIR, Project

BRANCH_INV = {("beq", "bne"), ("bne", "beq"), ("blt", "bge"), ("bge", "blt"), ("bgt", "ble"), ("ble", "bgt")}
FLOAT_PAIRS = {"fsubs", "fsub", "fadds", "fadd", "fmuls", "fmul", "fdivs", "fdiv", "fmadds", "fmadd", "fmsubs", "fmsub", "frsp"}

INT_TYPES = ["s8", "u8", "s16", "u16", "s32", "u32", "int", "unsigned int", "unsigned", "char", "unsigned char",
             "short", "unsigned short", "long", "unsigned long", "signed char"]
FLIP = {"s8": "u8", "u8": "s8", "s16": "u16", "u16": "s16", "s32": "u32", "u32": "s32",
        "int": "u32", "unsigned int": "s32", "unsigned": "s32", "char": "u8", "unsigned char": "s8", "signed char": "u8",
        "short": "u16", "unsigned short": "s16", "long": "u32", "unsigned long": "s32"}
WIDEN = {"s8": ["s16", "s32"], "u8": ["u16", "u32"], "s16": ["s8", "s32"], "u16": ["u8", "u32"],
         "s32": ["s16", "s8"], "u32": ["u16", "u8"], "int": ["s16", "s8"], "char": ["s16", "s32"], "short": ["s8", "s32"]}
TYPE_RE = "|".join(re.escape(t) for t in sorted(INT_TYPES, key=len, reverse=True))
# a declaration: type, then one or more declarators (pointers excluded: their signedness never matters)
DECL_RE = re.compile(rf"(?<![\w.>])(?:const\s+)?({TYPE_RE})\s+(?!\*)([A-Za-z_]\w*(?:\s*\[[^\]]*\])?)(?=\s*[;,=)\[])")


def _kinds(res: oracle.CheckResult) -> Dict[str, int]:
    lrows, rrows = getattr(res, "_rows", ([], []))
    return stuck.classify_rows(lrows, rrows)


def _wants_type_flip(counts: Dict[str, int], diffs: List[Tuple[str, str]]) -> bool:
    if any(k.startswith("op:cmp") for k in counts) or counts.get("ins:ext"):
        return True
    text = " ".join(t + " " + o for t, o in diffs)
    return bool(re.search(r"\b(extsh|extsb|clrlwi|cmplw|cmpw|cmplwi|cmpwi|rlwinm)\b", text)) or "@" in text


def _decl_sites(body: str, fn_span: Tuple[int, int]) -> List[Tuple[int, int, str, str]]:
    """(start, end, type, name) of integer declarations in the function and in structs above it."""
    out = []
    for m in DECL_RE.finditer(body):
        if m.start() > fn_span[1]:
            break
        typ, name = m.group(1), m.group(2).split("[")[0].strip()
        # must be used inside the function to matter
        if re.search(rf"\b{re.escape(name)}\b", body[fn_span[0]:fn_span[1]]):
            out.append((m.start(1), m.end(1), typ, name))
    return out


def _function_span(text: str, name: str) -> Optional[Tuple[int, int]]:
    m = re.search(rf"\b{re.escape(name)}\s*\([^;{{]*\)\s*\{{", text)
    if not m:
        return None
    depth, i = 0, m.end() - 1
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return m.start(), i + 1
        i += 1
    return None


def _tu_of(p: Project, sym) -> Optional[str]:
    """The TU file that holds this function per tus.json, carved or not."""
    try:
        for t in p.tu_map(sym.module).values() if isinstance(p.tu_map(sym.module), dict) else []:
            pass
    except Exception:
        pass
    import json
    path = p.module_config_dir(sym.module) / "tus.json"
    if not path.exists():
        return None
    try:
        d = json.loads(path.read_text())
    except ValueError:
        return None
    for t in d.get("tus", []):
        if sym.name in t.get("functions", []):
            return f"{p.module_src_prefix(sym.module)}/{t['file']}"
    return None


def try_fix(p: Project, symbol: str, body: str, budget_s: float = 30.0, max_candidates: int = 80, _depth: int = 0) -> Dict[str, object]:
    """Search the cheap repairs; returns {"matched": bool, "body": text or None, "tried": n, "best": %, "secs": s}."""
    t0 = time.time()
    sym = p.resolve(symbol)
    key = p.key(sym)
    scratch = STATE_DIR / "fixup" / (key.replace(":", "__") + ".c")
    scratch.parent.mkdir(parents=True, exist_ok=True)

    def check(text: str) -> oracle.CheckResult:
        scratch.write_text(text)
        return oracle.check(p, symbol, 0, source=scratch)

    base = check(body)
    out: Dict[str, object] = {"matched": False, "body": None, "tried": 0, "best": base.percent if base.ok else 0.0,
                              "base": base.percent if base.ok else 0.0, "secs": 0.0, "kinds": {}}
    if not base.ok:
        out["error"] = base.error[-300:]
        return out
    if base.matched or base.matched_pool:
        out.update(matched=True, body=body)
        return out
    counts = _kinds(base)
    out["kinds"] = {k: v for k, v in counts.items()}
    lrows, rrows = base._rows
    diffs = [(stuck._fmt(a), stuck._fmt(b)) for a, b in zip(lrows, rrows) if (a.get("diff_kind") or "DIFF_NONE") != "DIFF_NONE"]
    span = _function_span(body, sym.name)
    candidates: List[Tuple[str, str]] = []
    if span and _wants_type_flip(counts, diffs):
        sites = _decl_sites(body, span)
        for s, e, typ, name in sites:
            alts = [FLIP[typ]] if typ in FLIP else []
            if counts.get("ins:ext") or "@" in " ".join(t + o for t, o in diffs):
                alts += WIDEN.get(typ, [])
            for alt in alts:
                candidates.append((f"{name}:{typ}->{alt}", body[:s] + alt + body[e:]))
    # declaration variants: when a symbol this body declares is declared differently by another
    # block of the same TU (a contested prototype or extern type), each sibling variant is a
    # candidate: a matched neighbour usually already found the spelling the compiler wants
    tu_src = None
    try:
        rec = p.unit_record(p.unit_of(sym)) if p.unit_of(sym) else None
        tu_src = rec.get("tu") if rec else None
        if tu_src is None:
            tu_src = next((t for t in [_tu_of(p, sym)] if t), None)
    except Exception:
        tu_src = None
    if tu_src:
        from . import tufile, tutidy
        try:
            tf = tufile.load(p, tu_src)
        except Exception:
            tf = None
        if tf is not None:
            mine = {}
            for ln in body.splitlines():
                if tutidy.DECL_LINE_RE.match(ln):
                    n = tutidy._decl_name(ln)
                    if n:
                        mine.setdefault(n, ln.strip())
            variants: Dict[str, List[str]] = {}
            for b in tf.blocks:
                if b.name == sym.name:
                    continue
                for ln in b.body.splitlines():
                    if tutidy.DECL_LINE_RE.match(ln):
                        n = tutidy._decl_name(ln)
                        if n in mine and ln.strip() != mine[n] and ln.strip() not in variants.setdefault(n, []):
                            variants[n].append(ln.strip())
            for ln in tf.prologue.splitlines():
                if tutidy.DECL_LINE_RE.match(ln):
                    n = tutidy._decl_name(ln)
                    if n in mine and ln.strip() != mine[n] and ln.strip() not in variants.setdefault(n, []):
                        variants[n].append(ln.strip())
            for n, alts in variants.items():
                for alt in alts[:4]:
                    candidates.append((f"{n}: {mine[n]} -> {alt}", body.replace(mine[n], alt, 1)))
    # wrong callee / wrong data symbol: the same instruction with a different relocation target.
    # The retail name is known; the body names ours verbatim, so the substitution is exact.
    subs: Dict[str, str] = {}
    for t, o in diffs:
        if not t or not o or t.split()[0] != o.split()[0]:
            continue
        mt = re.findall(r"\b([A-Za-z_]\w*)(?=@|$|\b)", re.sub(r"^\S+\s+", "", t))
        mo = re.findall(r"\b([A-Za-z_]\w*)(?=@|$|\b)", re.sub(r"^\S+\s+", "", o))
        tn = [x for x in mt if not re.fullmatch(r"[rf]\d+|cr\d|lt|gt|eq|so|ha|l|sda21", x)]
        on = [x for x in mo if not re.fullmatch(r"[rf]\d+|cr\d|lt|gt|eq|so|ha|l|sda21", x)]
        if len(tn) == 1 and len(on) == 1 and tn[0] != on[0] and not on[0].startswith("@"):
            if re.sub(r"\b" + re.escape(on[0]) + r"\b", tn[0], o) == t:
                subs.setdefault(on[0], tn[0])
    for ours, retail in subs.items():
        if re.search(rf"\b{re.escape(ours)}\b", body) and not re.search(rf"\b{re.escape(retail)}\b", body):
            candidates.append((f"symbol {ours} -> {retail}", re.sub(rf"\b{re.escape(ours)}\b", retail, body)))
    if len(subs) > 1:
        text = body
        for ours, retail in subs.items():
            text = re.sub(rf"\b{re.escape(ours)}\b", retail, text)
        candidates.append(("all symbol substitutions", text))
    # float vs double: fsubs/fsub, frsp rows come from f32/f64 declarations and literal suffixes
    if any((t.split()[0] if t else "") in FLOAT_PAIRS or (o.split()[0] if o else "") in FLOAT_PAIRS or "frsp" in (t + o) for t, o in diffs):
        for a, b in (("f64", "f32"), ("f32", "f64"), ("double", "float"), ("float", "double")):
            if re.search(rf"\b{a}\b", body):
                candidates.append((f"all {a}->{b}", re.sub(rf"\b{a}\b", b, body)))
                for m in list(re.finditer(rf"\b{a}\b", body))[:12]:
                    candidates.append((f"{a}->{b} at {m.start()}", body[:m.start()] + b + body[m.end():]))
        lits = list(re.finditer(r"(?<![\w.])(\d+\.\d*(?:[eE][-+]?\d+)?)(?![\w.])", body))
        if lits:
            candidates.append(("float literals get f", re.sub(r"(?<![\w.])(\d+\.\d*(?:[eE][-+]?\d+)?)(?![\w.])", r"\1f", body)))
        litf = list(re.finditer(r"(?<![\w.])(\d+\.\d*(?:[eE][-+]?\d+)?)f\b", body))
        if litf:
            candidates.append(("float literals lose f", re.sub(r"(?<![\w.])(\d+\.\d*(?:[eE][-+]?\d+)?)f\b", r"\1", body)))
    # struct layout: every field offset off by the same delta means padding is missing or extra
    # at the front of the block-private struct; two deltas mean two fields are in the wrong order
    deltas = set()
    for t, o in diffs:
        if not t or not o or t.split()[0] != o.split()[0]:
            continue
        mt = re.search(r"(-?0x[0-9a-f]+|-?\d+)\((r\d+)\)", t); mo = re.search(r"(-?0x[0-9a-f]+|-?\d+)\((r\d+)\)", o)
        if mt and mo and mt.group(2) == mo.group(2) and mt.group(2) != "r1" and mt.group(1) != mo.group(1):
            deltas.add(int(mt.group(1), 0) - int(mo.group(1), 0))
    struct_spans = [(m.start(), m.end(), m.group(1)) for m in re.finditer(r"(?:typedef\s+)?struct\s+\w*\s*\{([^}]*)\}", body)]
    if len(deltas) == 1 and struct_spans:
        delta = next(iter(deltas))
        for s0, e0, inner in struct_spans:
            if delta > 0:
                new_inner = f"\n    u8 _pad_pre[0x{delta:X}];" + inner
                candidates.append((f"struct +{delta} front padding", body[:s0] + body[s0:e0].replace(inner, new_inner, 1) + body[e0:]))
            else:
                m = re.match(r"\s*u8\s+(\w+)\[(0x[0-9A-Fa-f]+|\d+)\];", inner)
                if m and int(m.group(2), 0) + delta >= 0:
                    n = int(m.group(2), 0) + delta
                    rep = "" if n == 0 else f"\n    u8 {m.group(1)}[0x{n:X}];"
                    candidates.append((f"struct {delta} front padding", body[:s0] + body[s0:e0].replace(inner, re.sub(r"^\s*u8\s+\w+\[[^\]]+\];", rep, inner, count=1), 1) + body[e0:]))
    if len(deltas) >= 2 and struct_spans:
        for s0, e0, inner in struct_spans:
            lines = inner.split("\n")
            fl = [i for i, ln in enumerate(lines) if re.match(r"\s*[A-Za-z_][\w ]*\*?\s*\w+(\[[^\]]*\])?;", ln)]
            for a, b in zip(fl, fl[1:]):
                sw = list(lines); sw[a], sw[b] = sw[b], sw[a]
                candidates.append((f"swap fields {lines[a].strip()} <-> {lines[b].strip()}", body[:s0] + body[s0:e0].replace(inner, "\n".join(sw), 1) + body[e0:]))
                if len(candidates) > max_candidates:
                    break
    # inverted branch: negate one `if` condition and swap its then/else blocks
    if any(t and o and (t.split()[0], o.split()[0]) in BRANCH_INV for t, o in diffs):
        for m in list(re.finditer(r"\bif\s*\(", body))[:16]:
            depth, i = 1, m.end()
            while i < len(body) and depth:
                depth += body[i] == "("; depth -= body[i] == ")"; i += 1
            cond = body[m.end():i - 1]
            j = i
            while j < len(body) and body[j] in " \t\r\n": j += 1
            if j >= len(body) or body[j] != "{":
                continue
            d2, k = 1, j + 1
            while k < len(body) and d2:
                d2 += body[k] == "{"; d2 -= body[k] == "}"; k += 1
            then_blk = body[j:k]
            rest = body[k:]
            me = re.match(r"\s*else\s*(\{)", rest)
            if me:
                d3, e = 1, k + me.end()
                while e < len(body) and d3:
                    d3 += body[e] == "{"; d3 -= body[e] == "}"; e += 1
                else_blk = body[k + me.end() - 1:e]
                neg = f"!({cond})" if not re.fullmatch(r"\s*!\((.*)\)\s*", cond) else re.fullmatch(r"\s*!\((.*)\)\s*", cond).group(1)
                text = body[:m.end()] + neg + ") " + else_blk + " else " + then_blk + body[e:]
                candidates.append((f"invert if at {m.start()}", text))
            else:
                # `if (c) { return A; } ... return B;` is equivalent to `if (!c) { rest } return A;` only in
                # simple shapes; the cheap variant that changes codegen: swap == / != in the condition
                if "==" in cond or "!=" in cond:
                    c2 = cond.replace("==", "\0").replace("!=", "==").replace("\0", "!=")
                    candidates.append((f"flip ==/!= at {m.start()}", body[:m.end()] + c2 + body[i - 1:]))
    best_text = None
    # every candidate compiles in one mwcc run (the process start dominates a single compile),
    # then each object is scored; the winner alone goes through the full check (pool rows etc.)
    cand = candidates[:max_candidates]
    if cand and time.time() - t0 < budget_s:
        bdir = STATE_DIR / "fixup" / "batch" / key.replace(":", "__")
        bdir.mkdir(parents=True, exist_ok=True)
        for old in bdir.glob("*.c"):
            old.unlink()
        srcs = []
        for i, (label, text) in enumerate(cand):
            f = bdir / f"c{i}.c"; f.write_text(text); srcs.append(f)
        target = p.target_object_for(sym)
        objs = oracle.compile_many(p, sym.module, srcs, bdir / "obj") if target else {}
        out["tried"] = len(cand)
        scored = []
        for i, (label, text) in enumerate(cand):
            o = objs.get(srcs[i])
            if o is None:
                continue
            ok_, pct = oracle.function_score(p, sym.name, target, o)
            scored.append((pct, i, label, text))
        scored.sort(key=lambda x: -x[0])
        if scored and scored[0][0] > out["best"]:
            pct, i, label, text = scored[0]
            r = check(text)  # the full verdict: pool rows, adjusted percent
            if r.ok:
                pct2 = r.percent_adjusted or r.percent
                if pct2 > out["best"]:
                    out["best"] = pct2; out["best_label"] = label; best_text = text
                if r.matched or r.matched_pool:
                    out.update(matched=True, body=text, label=label)
        # a pool match can hide behind a lower positional score: check the next few too
        if not out["matched"]:
            for pct, i, label, text in scored[1:4]:
                if pct < 90 or time.time() - t0 > budget_s:
                    break
                r = check(text)
                if r.ok and (r.matched or r.matched_pool):
                    out.update(matched=True, body=text, label=label, best=100.0)
                    break
    # a plateau usually has more than one cause: when a repair improved the body without
    # matching, search again from the improved body (greedy, bounded by the budget)
    if not out["matched"] and best_text is not None and out["best"] > out["base"] + 0.05 and _depth < 3:
        left = budget_s - (time.time() - t0)
        if left > 2:
            nxt = try_fix(p, symbol, best_text, budget_s=left, max_candidates=max_candidates, _depth=_depth + 1)
            out["tried"] += nxt["tried"]
            if nxt["best"] > out["best"]:
                out["best"] = nxt["best"]; out["best_label"] = f"{label} + {nxt.get('best_label')}"
            if nxt.get("matched"):
                out.update(matched=True, body=nxt["body"], label=f"{out.get('best_label')} + {nxt.get('label')}")
            out["rounds"] = 1 + nxt.get("rounds", 0)
    out["secs"] = round(time.time() - t0, 1)
    return out
