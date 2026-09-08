"""Carve functions into their own compilation units.

For each claimed function: add a `splits.txt` entry covering its `.text` range
(plus any data it references exclusively, when contiguous), create the stub
source file, and register the unit in config/<VERSION>/units.json as
`nonmatching` so the retail object keeps linking until `fzgx submit` flips it.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple

from .project import ROOT, Function, Project, Symbol

DATA_SECTIONS = {".rodata", ".data", ".sdata", ".sdata2", ".bss", ".sbss", ".sbss2"}


@dataclass
class CarveResult:
    symbol: str
    module: str
    source: str
    created: bool
    ranges: List[Tuple[str, int, int, int]] = field(default_factory=list)  # section,start,end,align
    notes: List[str] = field(default_factory=list)


def _align_for(addr: int, cap: int) -> int:
    a = 4
    while a * 2 <= cap and addr % (a * 2) == 0:
        a *= 2
    return a


def _section_default_align(project: Project, module: str, section: str) -> int:
    text = (project.module_config_dir(module) / "splits.txt").read_text()
    for line in text.splitlines():
        parts = line.split()
        if parts and parts[0] == section and "align:" in line:
            return int([p for p in parts if p.startswith("align:")][0][6:])
    return 4


def exclusive_data(project: Project, fn: Function) -> Dict[str, List[Symbol]]:
    """Data symbols referenced only by this function, grouped by section."""
    syms = project.symbols(fn.symbol.module)
    index = project.function_asm(fn.symbol.module)
    out: Dict[str, List[Symbol]] = defaultdict(list)
    for name in fn.refs:
        s = syms.get(name)
        if not s or s.kind != "object" or s.section not in DATA_SECTIONS or s.scope != "local":
            continue
        users = [f for f in index.values() if name in f.refs]
        if len(users) == 1:
            out[s.section].append(s)
    return out


def add_force_active(project: Project, module: str, symbol: str) -> bool:
    """Add SYMBOL to the module's `force_active` list in config.yml (text edit, comments kept).

    REL partial links run with -strip_partial; a carved function nobody
    references would be dropped from our link (retail never stripped it).
    dtk turns force_active into the ldscript's FORCEACTIVE block.
    """
    path = project.config_dir / "config.yml"
    lines = path.read_text().splitlines()
    if module == "main":
        start, end = 0, next((i for i, l in enumerate(lines) if l.startswith("modules:")), len(lines))
        indent = ""
    else:
        start = next(i for i, l in enumerate(lines) if l.strip() == f"name: {module}")
        end = next((i for i in range(start + 1, len(lines)) if lines[i].startswith("- object:")), len(lines))
        indent = "  "
    block = lines[start:end]
    for i, l in enumerate(block):
        if l.strip() == "force_active:":
            items = [x.strip()[2:] for x in block[i + 1:] if x.strip().startswith("- ")]
            if symbol in items:
                return False
            lines.insert(start + i + 1, f"{indent}- {symbol}")
            path.write_text("\n".join(lines) + "\n")
            return True
    # no list yet: insert right after the name line (module) or before modules: (DOL)
    at = start + 1 if module != "main" else end
    lines[at:at] = [f"{indent}force_active:", f"{indent}- {symbol}"]
    path.write_text("\n".join(lines) + "\n")
    return True


def carve(project: Project, symbol: str, dry_run: bool = False) -> CarveResult:
    sym = project.find_symbol(symbol)
    if sym is None or sym.kind != "function":
        raise LookupError(f"{symbol}: not a known function symbol")
    module = sym.module
    existing = project.unit_of(sym)
    prefix = project.module_src_prefix(module)
    source = f"{prefix}/{sym.name}.c"
    if existing:
        return CarveResult(sym.name, module, existing if existing.endswith(".c") else existing,
                           created=False, notes=[f"already in unit {existing}"])
    fn = project.function_asm(module).get(sym.name)
    if fn is None:
        raise LookupError(f"{symbol}: no disassembly found under build/ (run ninja first)")

    res = CarveResult(sym.name, module, source, created=True)
    res.ranges.append((".text", sym.addr, sym.end, 4))
    for section, objs in exclusive_data(project, fn).items():
        objs.sort(key=lambda s: s.addr)
        contiguous = all(objs[i].end <= objs[i + 1].addr <= objs[i].end + 32 for i in range(len(objs) - 1))
        if not contiguous:
            res.notes.append(f"{section}: exclusive data not contiguous ({', '.join(o.name for o in objs)}); left in auto unit")
            continue
        start, end = objs[0].addr, objs[-1].end
        if any(sp.section == section and sp.start < end and start < sp.end for sp in project.splits(module)):
            res.notes.append(f"{section}: 0x{start:X}-0x{end:X} overlaps an existing split; left in auto unit")
            continue
        cap = _section_default_align(project, module, section)
        res.ranges.append((section, start, end, _align_for(start, cap)))
    if dry_run:
        return res

    add_force_active(project, module, sym.name)
    splits_path = project.module_config_dir(module) / "splits.txt"
    with splits_path.open("a") as f:
        f.write(f"\n{source}:\n")
        for section, start, end, align in res.ranges:
            f.write(f"\t{section:<11} start:0x{start:08X} end:0x{end:08X} align:{align}\n")

    src_path = ROOT / "src" / source
    if not src_path.exists():
        src_path.parent.mkdir(parents=True, exist_ok=True)
        src_path.write_text(
            f'#include "types.h"\n\n'
            f"// {sym.name}: {module} {sym.section}:0x{sym.addr:08X} size 0x{sym.size:X}\n"
            f"// Carved by fzgx. Replace this file's body with the matching C.\n"
        )
    units = project.load_units()
    if not any(u["module"] == module and u["source"] == source for u in units):
        units.append({"module": module, "source": source, "symbols": [sym.name],
                      "status": "nonmatching", "mw_version": None, "extra_cflags": []})
        project.save_units(units)
    return res
