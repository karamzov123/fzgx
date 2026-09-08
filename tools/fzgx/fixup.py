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


def try_fix(p: Project, symbol: str, body: str, budget_s: float = 30.0, max_candidates: int = 60) -> Dict[str, object]:
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
    for label, text in candidates[:max_candidates]:
        if time.time() - t0 > budget_s:
            out["timeout"] = True
            break
        r = check(text)
        out["tried"] += 1
        if not r.ok:
            continue
        pct = r.percent_adjusted or r.percent
        if pct > out["best"]:
            out["best"] = pct
            out["best_label"] = label
        if r.matched or r.matched_pool:
            out.update(matched=True, body=text, label=label)
            break
    out["secs"] = round(time.time() - t0, 1)
    return out
