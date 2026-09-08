"""Naming bundles for the librarian, and application of its proposals.

bundle(p, module, tu) -> markdown the librarian model reads: the TU's file
name, every matched function's C source in address order, the names of
callers/callees (matched or not), string literals referenced, and the
naming rules. The model answers with JSON:

    {"renames": {"fn_1_6400": "camera_init", "lbl_1_bss_1018": "camera_state", ...},
     "structs": {"MainObject": "typedef struct { ... } CameraState;", ...},
     "notes": "..."}

apply(p, proposal) validates identifiers, refuses collisions, applies the
renames with one relink, and records accepted names in the ledger.
"""

from __future__ import annotations

import json
import re
from typing import Dict, List, Optional

from .ledger import Ledger
from .project import ROOT, Project
from .tu import rename_many, tu_map

RULES = """Rules: snake_case for functions and globals, PascalCase for struct typedefs; prefix game-code
functions with the TU stem when they are file-local helpers (e.g. camera_update_target); SDK/CRI/MSL
names verbatim if recognisable; never invent semantics the code does not show (prefer camera_set_field_0x3c
over camera_set_fov if the field's meaning is not evident); keep generated names for anything unclear;
do not rename symbols outside this TU's function list except globals whose meaning is unambiguous
from their use here; struct field names must preserve the exact offsets and sizes shown."""


def _data_context(p: Project, module: str):
    """Pointer-table references to functions and string contents, from dtk's data .s files.

    Returns (table_refs: fn -> [data symbol names containing a pointer to it],
             strings: data symbol -> literal text)."""
    import glob
    table_refs: Dict[str, List[str]] = {}
    strings: Dict[str, str] = {}
    cur = None
    for f in glob.glob(str(p.module_build_dir(module) / "asm" / "**" / "*.s"), recursive=True):
        for line in open(f, errors="replace"):
            m = re.match(r"^\.obj\s+(\S+?),", line)
            if m:
                cur = m.group(1)
                continue
            if cur is None:
                continue
            m = re.search(r'\.(?:asciz|string)\s+"((?:[^"\\]|\\.)*)"', line)
            if m and cur not in strings:
                strings[cur] = m.group(1)
                continue
            m = re.match(r"^\s*\.4byte\s+(fn_\w+|[A-Za-z_]\w*)\b", line)
            if m and (m.group(1).startswith("fn_") or p.find_symbol(m.group(1), module)):
                table_refs.setdefault(m.group(1), [])
                if cur not in table_refs[m.group(1)]:
                    table_refs[m.group(1)].append(cur)
    return table_refs, strings


def bundle(p: Project, module: str, tu: str, only_matched: bool = True) -> str:
    tmap = tu_map(p, module)
    stem = tu.rsplit(".", 1)[0]
    funcs = [f for f in p.functions(module) if tmap.get(f.name) == stem]
    units = {u["symbols"][0]: u for u in p.load_units() if u["module"] == module}
    l = Ledger()
    table_refs, strings = _data_context(p, module)
    tu_info = next((t for t in json.loads((p.module_config_dir(module) / "tus.json").read_text())["tus"]
                    if t["file"] == tu), None)
    parts = [f"# Naming task: translation unit `{stem}.c` in module `{module}`",
             f"{len(funcs)} functions in this TU; matched sources follow in address order. "
             f"Callers/callees are listed so cross-references get consistent names.", "", RULES, ""]
    idx = p.function_asm(module)
    # strings whose address falls in this TU's data block: the file's own literals
    if tu_info:
        d_lo, d_hi = tu_info["data"]
        sec_name = {4: ".rodata", 5: ".data"}.get(tu_info["data_sec"], ".data")
        own = [f'"{t}"' for name, t in strings.items()
               if (sym := p.find_symbol(name, module)) and sym.section == sec_name and d_lo <= sym.addr < d_hi]
        if own:
            parts.append("String literals in this TU's data block: " + ", ".join(own[:60]) + (" ..." if len(own) > 60 else ""))
            parts.append("")
    n = 0
    for f in funcs:
        u = units.get(f.name)
        row = l.get(p.key(f))
        matched = row is not None and row["status"] == "matched"
        if only_matched and not matched:
            continue
        n += 1
        fn = idx.get(f.name)
        callers = [c for c in p.callers(f.name, 6)]
        callees = [r for r in (fn.refs if fn else []) if r.startswith("fn_") or (p.find_symbol(r, module) and p.find_symbol(r, module).kind == "function")]
        tables = table_refs.get(f.name, [])
        tbl = ""
        if tables:
            tbl = "  referenced from data tables: " + ", ".join(
                f"{t} (in {tmap.get(t, '?')})" if t in tmap else t for t in tables[:4])
        data_refs = []
        for r in (fn.refs if fn else []):
            sdef = p.find_symbol(r, module)
            if sdef and sdef.kind == "object":
                data_refs.append(f'{r}="{strings[r]}"' if r in strings else r)
        parts.append(f"## `{f.name}`  ({f.section}:0x{f.addr:06X}, {f.size} B)"
                     + (f"  callers: {', '.join(callers)}" if callers else "  callers: none found")
                     + (f"  callees: {', '.join(callees[:8])}" if callees else "")
                     + tbl
                     + (f"  data: {', '.join(data_refs[:8])}" if data_refs else ""))
        if u:
            src = ROOT / "src" / u["source"]
            if src.exists():
                parts.append("```c\n" + src.read_text().strip() + "\n```")
        parts.append("")
    unmatched = [f.name for f in funcs if not (l.get(p.key(f)) or {"status": ""})["status"] == "matched"]
    if unmatched:
        parts.append(f"Unmatched functions in this TU (no source yet): {', '.join(unmatched[:40])}"
                     + (" ..." if len(unmatched) > 40 else ""))
    parts.append("")
    parts.append("Answer with JSON only: {\"renames\": {old: new, ...}, \"structs\": {TypedefName: \"typedef struct {...} Name;\"}, \"notes\": \"...\"}")
    return "\n".join(parts) + (f"\n\n<!-- {n} matched functions -->\n")


def apply(p: Project, proposal: Dict, proposed_by: str = "librarian") -> Dict[str, object]:
    renames: Dict[str, str] = dict(proposal.get("renames") or {})
    bad = {}
    for old, new in list(renames.items()):
        if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", new) or new == old:
            bad[old] = "invalid"; renames.pop(old)
        elif p.resolve(old) is None and p.find_symbol(old) is None:
            bad[old] = "unknown symbol"; renames.pop(old)
        elif any(new in p.symbols(m) for m in p.modules) or new in renames.values() and list(renames.values()).count(new) > 1:
            bad[old] = f"collision: {new}"; renames.pop(old)
    r = rename_many(p, renames) if renames else {"ok": True, "applied": 0, "results": []}
    l = Ledger()
    for old, new in renames.items():
        l.db.execute("INSERT INTO names(kind,target,proposed,rationale,proposed_by,status) VALUES('function',?,?,?,?,'applied')",
                     (new, new, proposal.get("notes", "")[:200], proposed_by))
    return {"ok": r.get("ok"), "applied": r.get("applied"), "rejected": bad,
            "structs": list((proposal.get("structs") or {}).keys())}
