"""Struct layout recovery for globals, from every access in a module's disassembly.

For a global G we look at each function that materialises G's address
(`lis rA, G@ha` / `addi rB, rA, G@l`, or a direct `lwz rB, G@l(rA)` when G
holds a pointer) and then follow the base register through memory accesses
`lwz/stw/lhz/lha/sth/lbz/stb/lfs/stfs/lfd/stfd rX, off(rB)` until rB is
overwritten. Two shapes emerge:

  object   G is the struct itself: fields at G+off
  pointer  G holds a pointer; fields are at (*G)+off

Output: {"kind": ..., "fields": {off: {"width": 4, "float": bool, "loads": n, "stores": n}},
         "users": [function names]}, plus a C typedef skeleton with fields
named by offset (`u32 unk_3C;`) that a librarian can rename.
"""

from __future__ import annotations

import re
from collections import defaultdict
from typing import Dict, List, Optional, Tuple

from .project import Project

ACCESS_RE = re.compile(r"^(l|st)(wz|w|hz|ha|h|bz|b|fs|fd)(u|x)?\s+(r|f)(\d+),\s*(-?0x[0-9a-fA-F]+|-?\d+)\((r\d+)\)")
WIDTH = {"wz": 4, "w": 4, "hz": 2, "h": 2, "ha": 2, "bz": 1, "b": 1, "fs": 4, "fd": 8}
OP_WIDTH = {"lwz": 4, "lhz": 2, "lha": 2, "lbz": 1, "lfs": 4, "lfd": 8, "stw": 4, "sth": 2, "stb": 1, "stfs": 4, "stfd": 8}
DEF_RE = re.compile(r"^(\w+)\s+r(\d+)")  # first operand register is the destination for most ops
NO_DEF = {"stw", "sth", "stb", "stfs", "stfd", "stwu", "stwx", "sthx", "stbx", "stfsx", "cmpw", "cmpwi", "cmplw",
          "cmplwi", "b", "bl", "beq", "bne", "blt", "bgt", "ble", "bge", "bdnz", "blr", "bctr", "bctrl", "mtctr", "mtlr"}


def _insn(line: str) -> str:
    return line.split(": ", 1)[1] if ": " in line else line


def analyze(p: Project, module: str, symbol: str) -> Dict[str, object]:
    fields: Dict[Tuple[str, int], Dict[str, object]] = defaultdict(lambda: {"width": 0, "float": False, "loads": 0, "stores": 0})
    users: List[str] = []
    kinds = defaultdict(int)
    for fn in p.function_asm(module).values():
        if symbol not in fn.refs:
            continue
        used = False
        base: Dict[str, str] = {}  # register -> "object" | "pointer"
        derived: Dict[str, tuple] = {}  # register -> ("field", kind, off): pointee of a pointer field
        pending_ha: Dict[str, bool] = {}
        for line in fn.asm:
            ins = _insn(line)
            m = re.match(rf"^lis r(\d+), {re.escape(symbol)}@ha", ins)
            if m:
                pending_ha[f"r{m.group(1)}"] = True
                continue
            m = re.match(rf"^addi r(\d+), r(\d+), {re.escape(symbol)}@l", ins)
            if m and f"r{m.group(2)}" in pending_ha:
                base[f"r{m.group(1)}"] = "object"
                continue
            # direct access through the symbol, with an optional displacement: op rX, (sym+0x3c)@l(rA) or sym@l(rA)
            m = re.match(rf"^(lwz|lhz|lha|lbz|lfs|lfd|stw|sth|stb|stfs|stfd) [rf](\d+), \(?{re.escape(symbol)}(?:\s*\+\s*(0x[0-9a-fA-F]+|\d+))?\)?@l\(r(\d+)\)", ins)
            if m and f"r{m.group(4)}" in pending_ha:
                off = int(m.group(3), 0) if m.group(3) else 0
                f = fields[("object", off)]
                f["width"] = max(f["width"], OP_WIDTH[m.group(1)])
                f["float"] = f["float"] or m.group(1) in ("lfs", "lfd", "stfs", "stfd")
                f["loads" if m.group(1).startswith("l") else "stores"] += 1
                if m.group(1) == "lwz":
                    base[f"r{m.group(2)}"] = "pointer"
                kinds["object"] += 1
                used = True
                continue
            m = ACCESS_RE.match(ins)
            if m:
                op, w, _, rf, rn, off, rb = m.groups()
                if rb in base:
                    off = int(off, 0)
                    key = (base[rb], off)
                    f = fields[key]
                    f["width"] = max(f["width"], WIDTH[w])
                    f["float"] = f["float"] or w in ("fs", "fd")
                    f["loads" if op == "l" else "stores"] += 1
                    kinds[base[rb]] += 1
                    used = True
                    if op == "l" and w in ("wz", "w") and rf == "r" and isinstance(base[rb], str):
                        # pointer field: the loaded register now addresses the pointee; record its fields under ("field", off)
                        dest = f"r{rn}"
                        tag = ("field", base[rb], off)
                        base.pop(dest, None)  # a load into the base register itself replaces it
                        derived[dest] = tag
                        pending_ha.pop(dest, None)
                        continue
                elif rb in derived:
                    off = int(off, 0)
                    key = (derived[rb], off)
                    f = fields[key]
                    f["width"] = max(f["width"], WIDTH[w])
                    f["float"] = f["float"] or w in ("fs", "fd")
                    f["loads" if op == "l" else "stores"] += 1
                    used = True
                if op == "l" and rf == "r":
                    dest = f"r{rn}"
                    if dest != rb or rb not in derived:
                        derived.pop(dest, None)
                    if dest in base and dest != rb:
                        base.pop(dest, None)
            d = DEF_RE.match(ins)
            if d and d.group(1) not in NO_DEF:
                reg = f"r{d.group(2)}"
                if reg in base:
                    base.pop(reg, None)
                if reg in derived and not (m and f"r{m.group(5)}" == reg and m.group(1) == "l" and m.group(7) in base):
                    derived.pop(reg, None)
                pending_ha.pop(reg, None)
        if used:
            users.append(fn.symbol.name)
    kind = "pointer" if kinds.get("pointer", 0) > kinds.get("object", 0) else "object"
    out = {}
    pointees: Dict[int, Dict[int, Dict]] = defaultdict(dict)
    for key, f in fields.items():
        if isinstance(key[0], tuple):  # ("field", kind, off), pointee_off
            (_, k, poff), off = key
            if k == kind:
                pointees[poff][off] = f
        elif key[0] == kind or (key[0] == "object" and key[1] == 0):
            out[key[1]] = f
    return {"symbol": symbol, "kind": kind, "fields": dict(sorted(out.items())), "users": users, "shapes": dict(kinds),
            "pointees": {poff: dict(sorted(fl.items())) for poff, fl in pointees.items() if sum(x["loads"] + x["stores"] for x in fl.values()) >= 3}}


def typedef(info: Dict[str, object], name: Optional[str] = None, fields: Optional[Dict[int, Dict]] = None) -> str:
    fields = fields if fields is not None else info["fields"]
    name = name or f"{info['symbol'].replace('lbl_', 'Struct_')}"
    lines = [f"typedef struct {{"]
    cur = 0
    for off in sorted(fields):
        f = fields[off]
        if off > cur:
            lines.append(f"    u8 pad_{cur:X}[0x{off - cur:X}];")
        elif off < cur:
            lines.append(f"    /* overlap at 0x{off:X} */")
        w = f["width"] or 4
        ctype = {1: "u8", 2: "u16", 4: "f32" if f["float"] else "u32", 8: "f64"}[w]
        lines.append(f"    {ctype} unk_{off:X};  // {f['loads']} loads, {f['stores']} stores")
        cur = off + w
    lines.append(f"}} {name};")
    return "\n".join(lines)


def header(p: Project, module: str, min_refs: int = 20, sections=(".data", ".bss", ".sdata", ".sbss")) -> str:
    """Generate a shared header for the most-referenced globals of a module."""
    from collections import Counter
    syms = p.symbols(module)
    cnt = Counter()
    for fn in p.function_asm(module).values():
        for r in fn.refs:
            sd = syms.get(r)
            if sd and sd.kind == "object" and sd.section in sections:
                cnt[r] += 1
    guard = f"REL_{module.upper()}_GLOBALS_H"
    out = [f"#ifndef {guard}", f"#define {guard}", "", '#include "types.h"', "",
           f"// Generated by `fzgx headers --module {module}`: layouts recovered from every access in the",
           "// module's disassembly (matched or not). Field names are offsets until the librarian names them;",
           "// offsets and widths are facts, names are not. Regenerate rather than hand-edit padding.", ""]
    for name, n in cnt.most_common():
        if n < min_refs:
            break
        sd = syms[name]
        info = analyze(p, module, name)
        tname = name.replace("lbl_", "Obj_")
        out.append(f"// {name}: {sd.section} size 0x{sd.size:X}, referenced by {n} functions, shape {info['shapes']}")
        nfields = info["fields"]
        if sd.size <= 8 and len(nfields) <= 1 and 0 in nfields or (sd.size <= 8 and not nfields):
            f = nfields.get(0, {"width": min(sd.size, 4) or 4, "float": False})
            ctype = {1: "u8", 2: "u16", 4: "f32" if f.get("float") else "u32", 8: "f64"}.get(f.get("width", 4), "u32")
            if info["kind"] == "pointer" and info.get("pointees", {}).get(0):
                pt = f"{tname}_Target"
                out.append(typedef(info, pt, info["pointees"][0]))
                out.append(f"extern {pt} *{name};")
            else:
                out.append(f"extern {ctype} {name};")
        else:
            out.append(typedef(info, tname))
            out.append(f"extern {tname} {name};")
            for poff, fl in sorted(info.get("pointees", {}).items()):
                if poff in nfields:
                    pt = f"{tname}_At{poff:X}"
                    out.append(f"// pointee of {name}.unk_{poff:X} (declare the field as {pt} * once named)")
                    out.append(typedef(info, pt, fl))
        out.append("")
    out.append(f"#endif  // {guard}")
    return "\n".join(out) + "\n"
