"""Mechanical C for straight-line functions: getters, setters, wrappers and short call-free
bodies, lifted from the retail disassembly. No model involved; the oracle verifies every
candidate and `fzgx trivial` submits the ones that match.

The lifter is a register-level symbolic evaluator over one basic block: constants, address
materialisation (lis/addi, @ha/@l), loads and stores by symbol or by base+offset (a struct
parameter), the common integer arithmetic, one call. Anything else makes it give up.
"""

from __future__ import annotations

import re
from typing import Dict, List, Optional, Tuple

from .project import Project

WIDTH = {"lwz": 4, "lhz": 2, "lha": 2, "lbz": 1, "lfs": 4, "lfd": 8, "stw": 4, "sth": 2, "stb": 1, "stfs": 4, "stfd": 8}
LOAD_T = {"lwz": "u32", "lhz": "u16", "lha": "s16", "lbz": "u8", "lfs": "f32", "lfd": "f64"}
STORE_T = {"stw": "u32", "sth": "u16", "stb": "u8", "stfs": "f32", "stfd": "f64"}
LINE_RE = re.compile(r"^[0-9A-Fa-f]+:\s*(\S+)\s*(.*)$")
MEM_RE = re.compile(r"^(-?0x[0-9a-f]+|-?\d+|[\w.]+@l|[\w.]+@sda21)\((r\d+)\)$")
SAVE_RE = re.compile(r"^r(1[4-9]|2\d|3[01])$")  # callee-saved: their save/restore is frame noise


class Give(Exception):
    pass


def _imm(s: str) -> int:
    try:
        return int(s, 0)
    except ValueError:
        raise Give()  # a symbol where a number was expected: not this lifter's shape


def lift(p: Project, module: str, name: str) -> Optional[str]:
    fa = p.function_asm(module).get(name)
    if fa is None:
        return None
    ins: List[Tuple[str, List[str]]] = []
    for ln in fa.asm:
        m = LINE_RE.match(ln.strip())
        if not m:
            continue
        mn, args = m.group(1), [a.strip() for a in m.group(2).split(",")] if m.group(2) else []
        ins.append((mn, args))
    if not ins or ins[-1][0] != "blr":
        return None
    if any(mn.startswith("b") and mn not in ("bl", "blr") for mn, _ in ins):
        return None  # one basic block only
    if len(ins) > 40:
        return None
    try:
        return _lift(p, module, name, ins)
    except Give:
        return None


def _lift(p: Project, module: str, name: str, ins) -> Optional[str]:
    syms = p.symbols(module)
    regs: Dict[str, str] = {}          # register -> C expression
    rtype: Dict[str, str] = {}         # register -> C type of the expression
    params: List[str] = []             # r3.. read before written
    ptypes: Dict[str, str] = {}        # param register -> type
    fields: Dict[str, Dict[int, str]] = {}   # param register -> {offset: type} (struct parameter)
    stmts: List[str] = []
    externs: Dict[str, str] = {}
    calls: List[str] = []
    frame = False
    ret = None
    r3_set = [False]  # r3 written by this function (a returned parameter counts once a call intervened)

    def use(r: str) -> str:
        if r in regs:
            return regs[r]
        if re.fullmatch(r"r([3-9]|10)", r) or re.fullmatch(r"f[1-8]", r):
            # parameters are contiguous from r3 (or f1): reading r5 implies r3 and r4 exist
            n = int(r[1:])
            base = 3 if r.startswith("r") else 1
            for k in range(base, n + 1):
                rk = f"{r[0]}{k}"
                if rk not in params:
                    params.append(rk)
                    ptypes[rk] = "f32" if rk.startswith("f") else "u32"
                    if rk not in regs:
                        regs[rk] = f"arg{params.index(rk)}"
                        rtype[rk] = ptypes[rk]
            return regs[r]
        if r == "r0":
            raise Give()
        raise Give()

    def sym_of(a: str) -> Optional[str]:
        m = re.match(r"^([\w.]+)@(ha|h|l|sda21)$", a)
        return m.group(1) if m else None

    far: set = set()  # data symbols retail addresses with lis/addi: declared with unknown size so
                      # MWCC's -sdata threshold (DOL) cannot move them into small data

    def declare(s: str, t: str, far_ref: bool = False) -> None:
        sd = syms.get(s)
        if sd is None:
            # dtk exports a TU-local symbol under its address-suffixed name; that is the name
            # the retail object relocates against, so it is the name the C must use
            m = re.match(r"^(.*)_[0-9A-F]{8}$", s)
            sd = syms.get(m.group(1)) if m else None
        if sd is None:
            raise Give()
        if sd.kind == "function":
            externs.setdefault(s, f"extern void {s}(void);")
        elif far_ref and module == "main":
            far.add(s)
            externs[s] = f"extern {t} {s}[];"
        else:
            externs.setdefault(s, f"extern {t} {s};")

    def ref(s: str) -> str:
        return f"{s}[0]" if s in far else s

    hi: Dict[str, str] = {}  # register holding sym@ha
    for i, (mn, a) in enumerate(ins):
        if mn == "blr":
            break
        if mn in ("stwu", "mflr", "mtlr") or (mn in ("stw", "lwz") and a and (a[0] == "r0" or SAVE_RE.match(a[0])) and "(r1)" in a[1]) or (mn == "addi" and a and a[0] == "r1"):
            frame = True
            continue
        if mn == "lis" and sym_of(a[1]):
            hi[a[0]] = sym_of(a[1]); regs.pop(a[0], None); continue
        if mn == "lis":
            regs[a[0]] = f"0x{(_imm(a[1]) & 0xFFFF) << 16:X}"; rtype[a[0]] = "u32"; continue
        if mn == "addi" and sym_of(a[2]) and a[1] in hi:
            s = sym_of(a[2]); declare(s, "u32"); regs[a[0]] = f"&{s}" if syms[s].kind != "function" else s; rtype[a[0]] = "void *"; continue
        if mn in ("li",):
            regs[a[0]] = str(_imm(a[1])); rtype[a[0]] = "s32"; continue
        if mn == "mr":
            regs[a[0]] = use(a[1]); rtype[a[0]] = rtype.get(a[1], "u32"); continue
        if mn in LOAD_T:
            m = MEM_RE.match(a[1])
            if not m:
                raise Give()
            off, base = m.group(1), m.group(2)
            t = LOAD_T[mn]
            if off.endswith("@l") and base in hi:
                s = hi[base]; declare(s, t, far_ref=True); regs[a[0]] = ref(s); rtype[a[0]] = t
            elif off.endswith("@sda21"):
                s = off[:-6]; declare(s, t); regs[a[0]] = s; rtype[a[0]] = t
            else:
                o = _imm(off)
                b = use(base)
                if base in params and not regs[base].startswith("("):
                    fields.setdefault(base, {})[o] = t
                    regs[a[0]] = f"{b}->unk_{o:X}"
                else:
                    raise Give()
                rtype[a[0]] = t
            continue
        if mn in STORE_T:
            m = MEM_RE.match(a[1])
            if not m:
                raise Give()
            off, base = m.group(1), m.group(2)
            t = STORE_T[mn]
            val = use(a[0])
            if off.endswith("@l") and base in hi:
                s = hi[base]; declare(s, t, far_ref=True); stmts.append(f"{ref(s)} = {val};")
            elif off.endswith("@sda21"):
                s = off[:-6]; declare(s, t); stmts.append(f"{s} = {val};")
            else:
                o = _imm(off); b = use(base)
                if base in params:
                    fields.setdefault(base, {})[o] = t
                    stmts.append(f"{b}->unk_{o:X} = {val};")
                else:
                    raise Give()
            continue
        if mn in ("extsh", "extsb", "clrlwi", "rlwinm", "slwi", "srwi", "srawi", "add", "subf", "sub", "mulli", "mullw", "neg", "or", "and", "xor", "ori", "andi.", "addis", "subi", "not", "extrwi", "extlwi"):
            d = a[0]
            if mn == "extrwi":
                n, b = _imm(a[2]), _imm(a[3]); regs[d] = f"(({use(a[1])} >> {32 - b - n}) & 0x{(1 << n) - 1:X})"; rtype[d] = "u32"; continue
            if mn == "extlwi":
                n, b = _imm(a[2]), _imm(a[3]); regs[d] = f"(({use(a[1])} << {b}) & 0x{((1 << n) - 1) << (32 - n):X})"; rtype[d] = "u32"; continue
            if mn == "extsh": regs[d] = f"(s16){use(a[1])}"; rtype[d] = "s16"
            elif mn == "extsb": regs[d] = f"(s8){use(a[1])}"; rtype[d] = "s8"
            elif mn == "clrlwi":
                n = 32 - _imm(a[2]); regs[d] = f"({use(a[1])} & 0x{(1 << n) - 1:X})" if n < 32 else use(a[1]); rtype[d] = "u32"
            elif mn == "slwi": regs[d] = f"({use(a[1])} << {_imm(a[2])})"; rtype[d] = "u32"
            elif mn == "srwi": regs[d] = f"((u32){use(a[1])} >> {_imm(a[2])})"; rtype[d] = "u32"
            elif mn == "srawi": regs[d] = f"((s32){use(a[1])} >> {_imm(a[2])})"; rtype[d] = "s32"
            elif mn == "add": regs[d] = f"({use(a[1])} + {use(a[2])})"; rtype[d] = "u32"
            elif mn in ("subf",): regs[d] = f"({use(a[2])} - {use(a[1])})"; rtype[d] = "u32"
            elif mn == "sub": regs[d] = f"({use(a[1])} - {use(a[2])})"; rtype[d] = "u32"
            elif mn == "subi": regs[d] = f"({use(a[1])} - {_imm(a[2])})"; rtype[d] = "u32"
            elif mn == "addi": regs[d] = f"({use(a[1])} + {_imm(a[2])})"; rtype[d] = "u32"
            elif mn == "mulli": regs[d] = f"({use(a[1])} * {_imm(a[2])})"; rtype[d] = "s32"
            elif mn == "mullw": regs[d] = f"({use(a[1])} * {use(a[2])})"; rtype[d] = "s32"
            elif mn == "neg": regs[d] = f"(-{use(a[1])})"; rtype[d] = "s32"
            elif mn == "or": regs[d] = f"({use(a[1])} | {use(a[2])})"; rtype[d] = "u32"
            elif mn == "and": regs[d] = f"({use(a[1])} & {use(a[2])})"; rtype[d] = "u32"
            elif mn == "xor": regs[d] = f"({use(a[1])} ^ {use(a[2])})"; rtype[d] = "u32"
            elif mn == "ori": regs[d] = f"({use(a[1])} | {_imm(a[2])})"; rtype[d] = "u32"
            elif mn == "andi.": regs[d] = f"({use(a[1])} & {_imm(a[2])})"; rtype[d] = "u32"
            elif mn == "not": regs[d] = f"(~{use(a[1])})"; rtype[d] = "u32"
            elif mn == "rlwinm":
                sh, mb, me = _imm(a[2]), _imm(a[3]), _imm(a[4])
                if sh == 0 and mb == 0:
                    n = me + 1; regs[d] = f"({use(a[1])} & 0x{(0xFFFFFFFF << (32 - n)) & 0xFFFFFFFF:X})"
                elif sh == 0 and me == 31:
                    regs[d] = f"({use(a[1])} & 0x{(1 << (32 - mb)) - 1:X})"
                elif mb == 0 and me == 31 - sh:
                    regs[d] = f"({use(a[1])} << {sh})"
                elif me == 31 and mb == 32 - sh and sh:
                    regs[d] = f"((u32){use(a[1])} >> {32 - sh})"
                else:
                    raise Give()
                rtype[d] = "u32"
            else:
                raise Give()
            continue
        if mn == "addi":  # plain addi (not an address)
            regs[a[0]] = f"({use(a[1])} + {_imm(a[2])})"; rtype[a[0]] = "u32"; continue
        if mn == "bl":
            callee = a[0]
            if callee not in syms:
                raise Give()
            # arguments: r3..rN where N is the highest argument register set here; a lower
            # register never written is a parameter of ours passed straight through
            set_regs = [k for k in range(3, 11) if f"r{k}" in regs]
            top = max(set_regs) if set_regs else 2
            args = [use(f"r{k}") for k in range(3, top + 1)]
            calls.append(callee)
            externs.setdefault(callee, f"extern u32 {callee}({', '.join(['u32'] * len(args)) or 'void'});")
            stmts.append(f"__CALL__{len(calls) - 1}({', '.join(args)});")
            for r in list(regs):
                if re.fullmatch(r"r([0-9]|1[0-2])|f([0-9]|1[0-3])", r):
                    regs.pop(r, None)
            regs["r3"] = f"__CALLRET__{len(calls) - 1}"; rtype["r3"] = "u32"
            continue
        raise Give()
    # return value: whatever r3 holds at blr, when this function wrote r3 (an untouched first
    # parameter is not a return value; a parameter copied back after a call is)
    wrote_r3 = any(a and a[0] == "r3" and mn not in ("stw", "sth", "stb", "stfs", "stfd", "cmpwi", "cmpw", "cmplwi", "cmplw") for mn, a in ins)
    if "r3" in regs and wrote_r3:
        ret = regs["r3"]  # a call's result falls through in r3 either way: `void f(void) { g(); }`
    # a call whose result is returned becomes `return f(...)`; one whose result feeds later code
    # becomes a temporary; the rest are statements
    used_ret = {i for i in range(len(calls)) if any(f"__CALLRET__{i}" in st for st in stmts if not st.startswith(f"__CALL__{i}(")) or ret == f"__CALLRET__{i}"}
    body = []
    for st in stmts:
        m = re.match(r"__CALL__(\d+)\((.*)\);", st)
        if m:
            i = int(m.group(1)); call = f"{calls[i]}({m.group(2)})"
            if ret == f"__CALLRET__{i}" and not any(f"__CALLRET__{i}" in x for x in stmts if x != st):
                body.append(f"return {call};"); ret = None
            elif i in used_ret:
                body.append(f"u32 t{i} = {call};")
            else:
                body.append(f"{call};")
        else:
            body.append(st)
    body = [re.sub(r"__CALLRET__(\d+)", r"t\1", b) for b in body]
    if ret is not None:
        ret = re.sub(r"__CALLRET__(\d+)", r"t\1", ret)
    # temporaries must be declared before any statement: hoist them
    decls = [b for b in body if b.startswith("u32 t")]
    if decls:
        names = [re.match(r"u32 (t\d+)", b).group(1) for b in decls]
        body = [f"u32 {', '.join(names)};"] + [re.sub(r"^u32 (t\d+) = ", r"\1 = ", b) for b in body]
    # an address stored or passed is a pointer: cast, so u32 fields and parameters accept it
    body = [re.sub(r"= (&[A-Za-z_]\w*(?:\[0\])?);", r"= (u32)\1;", b) for b in body]
    body = [re.sub(r"(\(|, )(&[A-Za-z_]\w*(?:\[0\])?)(?=[,)])", r"\1(u32)\2", b) for b in body]
    fnames = {s for s, e in externs.items() if e.startswith("extern void ") and e.endswith("(void);")}
    for f in fnames:
        body = [re.sub(rf"(= |\(|, ){re.escape(f)}(?=[,;)])", rf"\1(u32){f}", b) for b in body]
    if ret is not None:
        body.append(f"return {ret};")
    rtype_c = "void"
    if any(b.startswith("return ") for b in body):
        rtype_c = rtype.get("r3", "u32")
        if rtype_c in ("s8", "s16"): rtype_c = "s32"
    # parameters and struct parameters
    decl_params = []
    structs = []
    for i, r in enumerate(params):
        if r in fields:
            sname = f"{name}_Arg{i}"
            offs = fields[r]
            lines = [f"struct {sname} {{"]
            cur = 0
            for o in sorted(offs):
                if o > cur:
                    lines.append(f"    u8 pad_{cur:X}[0x{o - cur:X}];")
                w = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "f32": 4, "f64": 8}[offs[o]]
                lines.append(f"    {offs[o]} unk_{o:X};")
                cur = o + w
            lines.append("};")
            structs.append("\n".join(lines))
            decl_params.append(f"struct {sname} *arg{i}")
        else:
            decl_params.append(f"{ptypes[r]} arg{i}")
    if calls and not frame:
        raise Give()
    text = ['#include "types.h"', ""]
    text += sorted(externs.values())
    if structs:
        text += [""] + structs
    text += ["", f"{rtype_c} {name}({', '.join(decl_params) or 'void'}) {{"]
    text += [f"    {b}" for b in body]
    text += ["}", ""]
    return "\n".join(text)


def apply(p: Project, modules: Optional[List[str]] = None, max_size: int = 160, limit: int = 2000,
          workers: int = 12, submit: bool = True) -> Dict[str, object]:
    """Lift every unmatched function of the given size that the lifter accepts, check each
    against retail, submit the matches (carve-at-submit; `fzgx verify` relinks once)."""
    import sqlite3
    from concurrent.futures import ThreadPoolExecutor
    from . import api, oracle
    from .project import STATE_DIR
    db = sqlite3.connect(str(STATE_DIR / "ledger.db"))
    q = "select symbol, module, size from functions where status='unmatched' and size <= ?"
    args: List[object] = [max_size]
    if modules:
        q += " and module in (%s)" % ",".join("?" * len(modules)); args += list(modules)
    rows = db.execute(q + " order by size limit ?", args + [limit or 2000]).fetchall()
    lifted = []
    for s, m, size in rows:
        name = s.split(":", 1)[1] if ":" in s else s
        try:
            t = lift(p, m, name)
        except Exception:
            t = None
        if t:
            lifted.append((s, size, t))
    out_dir = STATE_DIR / "lift"; out_dir.mkdir(exist_ok=True)

    def check(item):
        s, size, t = item
        src = out_dir / (s.replace(":", "__") + ".c"); src.write_text(t)
        r = oracle.check(p, s, 4, source=src)
        return s, size, t, (r.ok and (r.matched or r.matched_pool) and oracle.unit_fully_matches(r) is None), (r.percent if r.ok else -1)
    with ThreadPoolExecutor(max_workers=workers) as ex:
        results = list(ex.map(check, lifted))
    matched = [(s, size, t) for s, size, t, ok, _ in results if ok]
    submitted, failed = [], []
    if submit:
        for s, size, t in matched:
            work = p.work_path(s if ":" in s else p.key(p.resolve(s)))
            work.parent.mkdir(parents=True, exist_ok=True)
            work.write_text(t)
            r = api.submit(p, s, agent="lift", message="lifted from the disassembly (fzgx trivial)", harness="fzgx", model="lift")
            (submitted if r.get("ok") else failed).append(s if r.get("ok") else (s, str(r.get("error"))[:80]))
    return {"candidates": len(rows), "lifted": len(lifted), "matched": len(matched), "bytes": sum(x[1] for x in matched),
            "submitted": len(submitted), "failed": failed[:10],
            "near": sorted(((s, round(pc, 1)) for s, _, _, ok, pc in results if not ok and pc >= 80), key=lambda x: -x[1])[:10]}
