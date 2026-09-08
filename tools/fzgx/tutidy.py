"""Remove block-private declarations that the TU's headers already provide.

Matched blocks written before the headers existed carry their own `extern`
lines and struct typedefs. Inside a per-function unit they are harmless; in the
whole-TU compile they collide with the headers and with each other. This pass
deletes, block by block, the private declarations the headers now cover, and
keeps each edit only if the block's generated unit still compiles and still
matches its retail object (plain or pool match). Nothing else is touched.
"""

from __future__ import annotations

import re
from typing import Dict, List, Optional, Set

from . import oracle, tufile
from .project import ROOT, Project

EXTERN_RE = re.compile(r"^\s*extern\b[^;{]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*;\s*$")
PROTO_RE = re.compile(r"^\s*(?:extern\s+)?[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*;\s*$")
TYPEDEF_START_RE = re.compile(r"^\s*typedef\s+struct\b")
TYPEDEF_END_RE = re.compile(r"^\s*\}\s*([A-Za-z_]\w*)\s*;\s*$")


def _header_names(p: Project, prologue: str) -> tuple:
    """(extern/prototype names, typedef names) declared by the prologue's includes."""
    names: Set[str] = set()
    typedefs: Set[str] = set()
    seen: Set[str] = set()

    def walk(rel: str) -> None:
        if rel in seen:
            return
        seen.add(rel)
        path = ROOT / "include" / rel
        if not path.exists():
            return
        text = path.read_text()
        for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', text, re.M):
            walk(m.group(1))
        for m in re.finditer(r"^extern\s+[^;(]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*;", text, re.M):
            names.add(m.group(1))
        for m in re.finditer(r"^extern\s+[^;]*?\b([A-Za-z_]\w*)\s*\(", text, re.M):
            names.add(m.group(1))
        for m in re.finditer(r"^\}\s*([A-Za-z_]\w*)\s*;", text, re.M):
            typedefs.add(m.group(1))
        for m in re.finditer(r"^typedef\s+[^{;]*?\b([A-Za-z_]\w*)\s*;", text, re.M):
            typedefs.add(m.group(1))

    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', prologue, re.M):
        walk(m.group(1))
    return names, typedefs


def tidy_block(body: str, names: Set[str], typedefs: Set[str]) -> tuple:
    """Drop covered externs/prototypes and duplicate struct typedefs. Returns (text, removed)."""
    out: List[str] = []
    removed: List[str] = []
    lines = body.splitlines()
    i = 0
    while i < len(lines):
        ln = lines[i]
        if TYPEDEF_START_RE.match(ln) and "{" in ln:
            j, depth = i, 0
            while j < len(lines):
                depth += lines[j].count("{") - lines[j].count("}")
                if depth <= 0 and j > i or (depth == 0 and TYPEDEF_END_RE.match(lines[j])):
                    break
                j += 1
            m = TYPEDEF_END_RE.match(lines[j]) if j < len(lines) else None
            if m and m.group(1) in typedefs:
                removed.append(f"typedef {m.group(1)}")
                i = j + 1
                continue
            out.extend(lines[i:j + 1])
            i = j + 1
            continue
        m = EXTERN_RE.match(ln) or (PROTO_RE.match(ln) if ln.lstrip().startswith("extern") else None)
        if m and m.group(1) in names:
            removed.append(m.group(1))
            i += 1
            continue
        out.append(ln)
        i += 1
    text = "\n".join(out).strip("\n") + "\n"
    text = re.sub(r"\n{3,}", "\n\n", text)
    return text, removed


def tidy(p: Project, tu_source: str, dry_run: bool = False) -> Dict[str, object]:
    module = tu_source.split("/")[1] if tu_source.startswith("rel/") else "main"
    tf = tufile.load(p, tu_source)
    names, typedefs = _header_names(p, tf.prologue)
    units = {u["symbols"][0]: u for u in p.load_units() if u.get("tu") == tu_source}
    kept, reverted, untouched = [], [], 0
    prologue_incs = {ln.strip() for ln in tf.prologue.splitlines() if tufile.INCLUDE_RE.match(ln)}
    for b in list(tf.blocks):
        inc, rest = tufile.split_includes(b.body)
        # a noprologue block is measured against the TU headers too: the point is to make it
        # compile under the prologue, so its private copies of header declarations must go
        new, removed = tidy_block(rest, names, typedefs)
        keep_inc = [ln for ln in inc if ln.strip() not in prologue_incs]
        was_noprologue = "noprologue" in b.flags
        if not removed and not was_noprologue:
            untouched += 1
            continue
        if dry_run:
            kept.append((b.name, removed))
            continue
        u = units.get(b.name)
        if u is None:
            continue
        old_body, old_flags = b.body, list(b.flags)
        b.body = ("\n".join(keep_inc) + "\n\n" + new) if keep_inc else new
        b.flags = [f for f in b.flags if f != "noprologue"]
        tufile._write_atomic(tufile.tu_path(p, tu_source), tf.render())
        tufile.write_gen(p, u, tf)
        res = oracle.check(p, b.name, 20)
        ok = res.ok and (res.matched or res.matched_pool) and oracle.unit_fully_matches(res) is None
        if ok:
            kept.append((b.name, removed + (["noprologue"] if was_noprologue else [])))
        else:
            b.body, b.flags = old_body, old_flags
            tufile._write_atomic(tufile.tu_path(p, tu_source), tf.render())
            tufile.write_gen(p, u, tf)
            reverted.append((b.name, removed, res.error[-160:] if not res.ok else f"{res.percent:.1f}%"))
    return {"tu": tu_source, "tidied": kept, "reverted": reverted, "untouched": untouched}
