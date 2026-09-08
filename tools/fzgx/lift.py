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
LOAD_T = {"lwz": "u32", "lhz": "u16", "lha": "s16", "lbz": "u8", "lfs": "f32", "lfd": "f64",
          "lwzu": "u32", "lhzu": "u16", "lbzu": "u8", "lfsu": "f32", "lfdu": "f64"}
STORE_T = {"stw": "u32", "sth": "u16", "stb": "u8", "stfs": "f32", "stfd": "f64"}
LABELS: List[Dict[str, int]] = [{}]
LINE_RE = re.compile(r"^[0-9A-Fa-f]+:\s*(\S+)\s*(.*)$")
MEM_RE = re.compile(r"^(-?0x[0-9a-f]+|-?\d+|[\w.]+@l|[\w.]+@sda21)\((r\d+)\)$")
COND = {"eq": "==", "ne": "!=", "lt": "<", "gt": ">", "le": "<=", "ge": ">="}
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
    labels: Dict[str, int] = {}
    for ln in fa.asm:
        t = ln.strip()
        if t.startswith(".L_") and t.endswith(":"):
            labels[t[:-1]] = len(ins)  # index of the next instruction
            continue
        m = LINE_RE.match(t)
        if not m:
            continue
        mn, args = m.group(1), [a.strip() for a in m.group(2).split(",")] if m.group(2) else []
        ins.append((mn, args))
    LABELS[0] = labels
    if not ins or ins[-1][0] != "blr":
        return None
    # control flow accepted: straight line, conditional returns (`beqlr` and friends), and
    # forward conditional branches whose target is the final blr or a later point of the same
    # straight-line body (an `if` block). Anything else (loops, several branches) is not lifted.
    branches = [(i, mn, a) for i, (mn, a) in enumerate(ins) if mn.startswith("b") and mn not in ("bl", "blr")]
    for i, mn, a in branches:
        if mn.endswith("lr") and mn[1:-2] in COND:
            continue
        if mn == "b" and a and a[-1].startswith(".L_") and labels.get(a[-1], -1) > i:
            continue  # a forward jump: the end of a then-block (checked again when lifted)
        m = re.fullmatch(r"b(\w+)", mn)
        if not (m and m.group(1) in COND and a and a[-1].startswith(".L_")):
            return None
    if len(ins) > 48:
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
        m = re.match(r"^([\w.]+)(?:[+-]0x[0-9a-fA-F]+)?@(ha|h|l|sda21)$", a)
        return m.group(1) if m else None

    def sym_off(a: str) -> int:
        m = re.match(r"^[\w.]+([+-]0x[0-9a-fA-F]+)?@", a)
        return int(m.group(1), 16) if m and m.group(1) else 0

    gfields: Dict[str, Dict[int, str]] = {}   # global symbol -> {offset: type} accessed as a struct
    pfields: Dict[str, Dict[int, str]] = {}   # pointer global -> {offset: type} accessed through it
    ptr_globals: set = set()

    far: set = set()  # data symbols retail addresses with lis/addi: declared with unknown size so
                      # MWCC's -sdata threshold (DOL) cannot move them into small data

    def lookup(s: str):
        sd = syms.get(s)
        if sd is None:
            # dtk exports a TU-local symbol under its address-suffixed name; that is the name
            # the retail object relocates against, so it is the name the C must use
            m = re.match(r"^(.*)_[0-9A-F]{8}$", s)
            sd = syms.get(m.group(1)) if m else None
        if sd is None:
            sd = p.find_symbol(s)  # another module: a REL imports from main.rel and the DOL
        return sd

    def declare(s: str, t: str, far_ref: bool = False) -> None:
        sd = lookup(s)
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


    locals_: Dict[str, str] = {}  # local pointer name -> global symbol it points at
    by_addr = sorted((sd.addr, sd) for sd in syms.values() if sd.kind == "object" and sd.size) if module == "main" else []

    def symbol_at(addr: int):
        """(symbol, offset) of the data object containing an absolute address, or None."""
        import bisect
        i = bisect.bisect_right([a for a, _ in by_addr], addr) - 1
        if i >= 0:
            sd = by_addr[i][1]
            if sd.addr <= addr < sd.addr + sd.size:
                return sd, addr - sd.addr
        return None

    def const_of(b: str) -> Optional[int]:
        m = re.fullmatch(r"0x([0-9A-Fa-f]+)", b) or re.fullmatch(r"\((0x[0-9A-Fa-f]+) \+ (-?\d+)\)", b)
        if not m:
            return None
        return int(m.group(1), 16) + (int(m.group(2)) if m.lastindex and m.lastindex >= 2 else 0)

    def field_base(b: str, base_reg: str):
        """Where a base+offset access lands: ("param", reg) / ("global", sym, k) / ("ptr", name) / None."""
        if base_reg in params and re.fullmatch(r"arg\d+", b):
            return ("param", base_reg, 0)
        if b in locals_:
            return ("global", locals_[b], 0)
        m = re.fullmatch(r"&([A-Za-z_]\w*)", b)
        if m:
            return ("global", m.group(1), 0)
        m = re.fullmatch(r"\(\(u8 \*\)&([A-Za-z_]\w*) \+ (\d+)\)", b)
        if m:
            return ("global", m.group(1), int(m.group(2)))
        if re.fullmatch(r"[A-Za-z_]\w*", b) and b in externs:
            return ("ptr", b, 0)
        c = const_of(b)
        if c is not None:
            return ("abs", c, 0)
        return None

    hi: Dict[str, str] = {}  # register holding sym@ha
    labels = LABELS[0]
    temps: List[str] = []

    def reads(idx: int, r: str) -> bool:
        """Is register r read at instruction idx (before being written there)?"""
        mn_, a_ = ins[idx]
        if not a_:
            return False
        srcs = a_[1:] if mn_ not in STORE_T and not mn_.startswith(("st", "cmp")) else a_
        return any(re.search(rf"\b{r}\b", x) for x in srcs)

    def reused_after_store(idx: int, r: str, mem: str) -> bool:
        """After instruction idx, is there a store to `mem` followed by a read of r, with no
        write of r in between? Then the source kept the loaded value in a local."""
        stored = False
        for j in range(idx + 1, len(ins)):
            mn_, a_ = ins[j]
            if mn_ in STORE_T and len(a_) > 1 and a_[1] == mem:
                stored = True
                continue
            if stored and reads(j, r):
                return True
            if mn_ == "bl":
                if stored and re.fullmatch(r"r([3-9]|10)", r):
                    return True  # an argument register at a call is read by the callee
                if re.fullmatch(r"r([0-9]|1[0-2])", r):
                    return False
            if a_ and a_[0] == r and mn_ not in STORE_T and not mn_.startswith(("st", "cmp")):
                return False  # r written again
        return False
    cond: Optional[Tuple[str, str, bool]] = None  # (lhs, rhs, unsigned) of the last compare
    open_ifs: List[Tuple[int, str]] = []          # (instruction index where a block closes, text to emit)
    skip: set = set()                             # instruction indices consumed by the structure (the `b` of a then-block)
    i = -1
    temps_written: List[Tuple[str, int]] = []
    for i, (mn, a) in enumerate(ins):
        if a and mn not in STORE_T and not mn.startswith(("st", "cmp", "b")) and mn not in ("mtlr", "mtspr"):
            temps_written.append((a[0], i))
        while open_ifs and open_ifs[-1][0] == i:
            stmts.append(open_ifs.pop()[1])
        if i in skip:
            continue
        if mn == "b":
            raise Give()  # an unconditional jump that no if/else explained
        if mn == "blr":
            break
        if mn in ("cmpwi", "cmpw", "cmplwi", "cmplw"):
            lhs = use(a[0]); rhs = str(_imm(a[1])) if mn.endswith("i") else use(a[1])
            uns = mn.startswith("cmpl")
            # the compare's signedness is the operands' type: cmpwi wants signed operands
            def typed(e: str, reg: str) -> str:
                t = rtype.get(reg, "u32")
                if uns:
                    return e if t in ("u32", "u16", "u8") else f"(u32){e}"
                return e if t in ("s32", "s16", "s8") else f"(s32){e}"
            lhs = typed(lhs, a[0])
            if not mn.endswith("i"):
                rhs = typed(rhs, a[1])
            cond = (lhs, rhs, uns); continue
        if mn.endswith("lr") and mn[1:-2] in COND and cond is not None:
            op = COND[mn[1:-2]]
            l, r_, uns = cond
            stmts.append(f"if ({l} {op} {r_}) {{ return __RET__; }}"); continue
        m = re.fullmatch(r"b(\w+)", mn)
        if m and m.group(1) in COND and a and a[-1].startswith(".L_") and cond is not None:
            tgt = labels.get(a[-1])
            if tgt is None or tgt <= i:
                raise Give()
            op = COND[m.group(1)]
            inv = {"==": "!=", "!=": "==", "<": ">=", ">": "<=", "<=": ">", ">=": "<"}[op]  # branch taken = skip
            l, r_, uns = cond
            if ins[tgt][0] == "blr" or tgt == len(ins) - 1:
                stmts.append(f"if ({l} {op} {r_}) {{ return __RET__; }}")
            else:
                # `if (c) { then } else { else }` when the then-block ends with a forward jump
                # over the else-block; otherwise a plain if
                pm, pa = ins[tgt - 1]
                if pm == "b" and pa and pa[-1].startswith(".L_") and labels.get(pa[-1], -1) > tgt:
                    end = labels[pa[-1]]
                    skip.add(tgt - 1)
                    stmts.append(f"if ({l} {inv} {r_}) {{")
                    # closers are pushed innermost-last: the stack pops the else first, then the end
                    open_ifs.append((end, "}")); open_ifs.append((tgt - 1, "} else {"))
                else:
                    stmts.append(f"if ({l} {inv} {r_}) {{"); open_ifs.append((tgt, "}"))
            continue
        if mn in ("stwu", "mflr", "mtlr") or (mn in ("stw", "lwz") and a and (a[0] == "r0" or SAVE_RE.match(a[0])) and "(r1)" in a[1]) or (mn == "addi" and a and a[0] == "r1"):
            frame = True
            continue
        if mn in ("stfd", "lfd", "psq_st", "psq_l") and a and re.fullmatch(r"f(1[4-9]|2\d|3[01])", a[0]) and "(r1)" in a[1]:
            frame = True
            continue  # callee-saved float registers
        if mn in ("crclr", "crset") or mn == "nop":
            continue  # condition-register housekeeping around varargs calls: no source
        if mn == "lis" and sym_of(a[1]):
            hi[a[0]] = sym_of(a[1]); regs.pop(a[0], None); continue
        if mn == "lis":
            regs[a[0]] = f"0x{(_imm(a[1]) & 0xFFFF) << 16:X}"; rtype[a[0]] = "u32"; continue
        if mn == "addi" and sym_of(a[2]) and a[1] in hi:
            s = sym_of(a[2]); declare(s, "u32")
            sd = lookup(s)
            so = sym_off(a[2])
            if sd is not None and sd.kind == "function":
                regs[a[0]] = s
            elif so:
                regs[a[0]] = f"((u8 *)&{s} + {so})"
            elif SAVE_RE.match(a[0]):
                # kept in a callee-saved register: the source held it in a local pointer
                ln = f"p_{s}"
                locals_[ln] = s
                declare(s, "struct", far_ref=True); gfields.setdefault(s, {})
                # the cast is what keeps the address in the register across calls: MWCC
                # rematerialises a plain `&sym` after each call, but not a cast of it
                stmts.append(f"{ln} = (struct {name}_{s} *)&{s};")
                regs[a[0]] = ln
            else:
                regs[a[0]] = f"&{s}"
            rtype[a[0]] = "void *"; continue
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
                s = hi[base]; so = sym_off(off)
                if so:
                    declare(s, "struct", far_ref=True); gfields.setdefault(s, {})[so] = t; regs[a[0]] = f"{s}.unk_{so:X}"
                else:
                    declare(s, t, far_ref=True); regs[a[0]] = ref(s)
                rtype[a[0]] = t
                if mn.endswith("u"):
                    regs[base] = f"&{s}" if not so else f"((u8 *)&{s} + {so})"; hi.pop(base, None)
                continue
            elif off.endswith("@sda21"):
                s = sym_of(off); so = sym_off(off)
                if so:
                    declare(s, "struct"); gfields.setdefault(s, {})[so] = t; regs[a[0]] = f"{s}.unk_{so:X}"
                else:
                    declare(s, t); regs[a[0]] = s
                rtype[a[0]] = t
            else:
                o = _imm(off)
                b = use(base)
                fb = field_base(b, base)
                if fb is None:
                    raise Give()
                kind, key, k = fb
                if kind == "param":
                    fields.setdefault(key, {})[o] = t; regs[a[0]] = f"{b}->unk_{o:X}"
                elif kind == "global" and b in locals_:
                    gfields.setdefault(key, {})[o] = t; regs[a[0]] = f"{b}->unk_{o:X}"
                elif kind == "global":
                    declare(key, "struct", far_ref=True); gfields.setdefault(key, {})[o + k] = t; regs[a[0]] = f"{key}.unk_{o + k:X}"
                elif kind == "abs":
                    hit = symbol_at(key + o)
                    if hit is None:
                        raise Give()  # hardware or unnamed memory: nothing the lint would accept
                    sd, so = hit
                    if so == 0 and sd.size <= 8:
                        declare(sd.name, t, far_ref=True); regs[a[0]] = ref(sd.name)
                    else:
                        declare(sd.name, "struct", far_ref=True); gfields.setdefault(sd.name, {})[so] = t; regs[a[0]] = f"{sd.name}.unk_{so:X}"
                else:
                    ptr_globals.add(key); pfields.setdefault(key, {})[o] = t; regs[a[0]] = f"{key}->unk_{o:X}"
                rtype[a[0]] = t
            if mn.endswith("u"):  # update form: the base register advances
                regs[base] = f"((u8 *){use(base)} + {_imm(off) if not off.endswith(('@l', '@sda21')) else 0})"
            if reused_after_store(i, a[0], a[1]):
                tn = f"v{len(temps)}"; temps.append(f"{rtype.get(a[0], 'u32')} {tn};")
                stmts.append(f"{tn} = {regs[a[0]]};"); regs[a[0]] = tn
            continue
        if mn in STORE_T:
            m = MEM_RE.match(a[1])
            if not m:
                raise Give()
            off, base = m.group(1), m.group(2)
            t = STORE_T[mn]
            val = use(a[0])
            if off.endswith("@l") and base in hi:
                s = hi[base]; so = sym_off(off)
                if so:
                    declare(s, "struct", far_ref=True); gfields.setdefault(s, {})[so] = t; stmts.append(f"{s}.unk_{so:X} = {val};")
                else:
                    declare(s, t, far_ref=True); stmts.append(f"{ref(s)} = {val};")
            elif off.endswith("@sda21"):
                s = sym_of(off); so = sym_off(off)
                if so:
                    declare(s, "struct"); gfields.setdefault(s, {})[so] = t; stmts.append(f"{s}.unk_{so:X} = {val};")
                else:
                    declare(s, t); stmts.append(f"{s} = {val};")
            else:
                o = _imm(off); b = use(base)
                fb = field_base(b, base)
                if fb is None:
                    raise Give()
                kind, key, k = fb
                if kind == "param":
                    fields.setdefault(key, {})[o] = t; stmts.append(f"{b}->unk_{o:X} = {val};")
                elif kind == "global" and b in locals_:
                    gfields.setdefault(key, {})[o] = t; stmts.append(f"{b}->unk_{o:X} = {val};")
                elif kind == "global":
                    declare(key, "struct", far_ref=True); gfields.setdefault(key, {})[o + k] = t; stmts.append(f"{key}.unk_{o + k:X} = {val};")
                elif kind == "abs":
                    hit = symbol_at(key + o)
                    if hit is None:
                        raise Give()
                    sd, so = hit
                    if so == 0 and sd.size <= 8:
                        declare(sd.name, t, far_ref=True); stmts.append(f"{ref(sd.name)} = {val};")
                    else:
                        declare(sd.name, "struct", far_ref=True); gfields.setdefault(sd.name, {})[so] = t; stmts.append(f"{sd.name}.unk_{so:X} = {val};")
                else:
                    ptr_globals.add(key); pfields.setdefault(key, {})[o] = t; stmts.append(f"{key}->unk_{o:X} = {val};")
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
            elif mn == "add":
                x, y = use(a[1]), use(a[2])
                # address + integer: byte arithmetic, or the pointee size scales the sum
                if rtype.get(a[1]) == "void *" and rtype.get(a[2]) != "void *":
                    regs[d] = f"((u8 *){x} + {y})"; rtype[d] = "void *"
                elif rtype.get(a[2]) == "void *" and rtype.get(a[1]) != "void *":
                    regs[d] = f"((u8 *){y} + {x})"; rtype[d] = "void *"
                else:
                    regs[d] = f"({x} + {y})"; rtype[d] = "u32"
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
            if rtype.get(a[1]) == "void *":
                regs[a[0]] = f"((u8 *){use(a[1])} + {_imm(a[2])})"; rtype[a[0]] = "void *"
            else:
                regs[a[0]] = f"({use(a[1])} + {_imm(a[2])})"; rtype[a[0]] = "u32"
            continue
        if mn == "oris":
            regs[a[0]] = f"({use(a[1])} | 0x{_imm(a[2]) << 16:X})"; rtype[a[0]] = "u32"; continue
        if mn == "subfic":
            regs[a[0]] = f"({_imm(a[2])} - {use(a[1])})"; rtype[a[0]] = "s32"; continue
        if mn == "clrrwi":
            n = _imm(a[2]); regs[a[0]] = f"({use(a[1])} & ~0x{(1 << n) - 1:X})"; rtype[a[0]] = "u32"; continue
        if mn == "clrlslwi":
            b, n = _imm(a[2]), _imm(a[3]); regs[a[0]] = f"(({use(a[1])} & 0x{(1 << (32 - b)) - 1:X}) << {n})"; rtype[a[0]] = "u32"; continue
        if mn in ("lwzx", "lhzx", "lbzx", "lfsx"):
            t = {"lwzx": "u32", "lhzx": "u16", "lbzx": "u8", "lfsx": "f32"}[mn]
            b = use(a[1]); i2 = use(a[2])
            regs[a[0]] = f"*({t} *)((u8 *){b} + {i2})"; rtype[a[0]] = t; continue
        if mn in ("stwx", "sthx", "stbx", "stfsx"):
            t = {"stwx": "u32", "sthx": "u16", "stbx": "u8", "stfsx": "f32"}[mn]
            stmts.append(f"*({t} *)((u8 *){use(a[1])} + {use(a[2])}) = {use(a[0])};"); continue
        if mn in ("fmuls", "fadds", "fsubs", "fdivs", "fmul", "fadd", "fsub", "fdiv"):
            op = {"fmuls": "*", "fadds": "+", "fsubs": "-", "fdivs": "/", "fmul": "*", "fadd": "+", "fsub": "-", "fdiv": "/"}[mn]
            t = "f32" if mn.endswith("s") else "f64"
            regs[a[0]] = f"({use(a[1])} {op} {use(a[2])})"; rtype[a[0]] = t; continue
        if mn in ("fmadds", "fmadd"):
            regs[a[0]] = f"(({use(a[1])} * {use(a[2])}) + {use(a[3])})"; rtype[a[0]] = "f32" if mn.endswith("s") else "f64"; continue
        if mn in ("fmsubs", "fmsub"):
            regs[a[0]] = f"(({use(a[1])} * {use(a[2])}) - {use(a[3])})"; rtype[a[0]] = "f32" if mn.endswith("s") else "f64"; continue
        if mn == "fmr":
            regs[a[0]] = use(a[1]); rtype[a[0]] = rtype.get(a[1], "f32"); continue
        if mn == "fneg":
            regs[a[0]] = f"(-{use(a[1])})"; rtype[a[0]] = rtype.get(a[1], "f32"); continue
        if mn == "frsp":
            regs[a[0]] = f"(f32){use(a[1])}"; rtype[a[0]] = "f32"; continue
        if mn == "nor":
            regs[a[0]] = f"(~({use(a[1])} | {use(a[2])}))"; rtype[a[0]] = "u32"; continue
        if mn == "slw":
            regs[a[0]] = f"({use(a[1])} << {use(a[2])})"; rtype[a[0]] = "u32"; continue
        if mn == "srw":
            regs[a[0]] = f"((u32){use(a[1])} >> {use(a[2])})"; rtype[a[0]] = "u32"; continue
        if mn == "sraw":
            regs[a[0]] = f"((s32){use(a[1])} >> {use(a[2])})"; rtype[a[0]] = "s32"; continue
        if mn == "mulhw":
            regs[a[0]] = f"(u32)(((s64){use(a[1])} * (s64){use(a[2])}) >> 32)"; rtype[a[0]] = "s32"; continue
        if mn == "cntlzw":
            regs[a[0]] = f"__cntlzw({use(a[1])})"; rtype[a[0]] = "u32"; continue
        if mn == "bl":
            callee = a[0]
            if lookup(callee) is None:
                raise Give()
            # arguments: r3..rN where N is the highest argument register set here; a lower
            # register never written is a parameter of ours passed straight through
            set_regs = [k for k in range(3, 11) if f"r{k}" in regs]
            top = max(set_regs) if set_regs else 2
            # registers below the lowest temporary this function used are parameters passed
            # straight through to the callee: a temporary in r6 with r3..r5 untouched means
            # the source wrote callee(a, b, c)
            written = [int(x[1:]) for x, _ in temps_written if re.fullmatch(r"r([3-9]|10)", x)]
            temp_low = min((w for w in written if f"r{w}" not in params), default=None)
            if temp_low is not None and temp_low - 1 > top:
                top = temp_low - 1
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
    if "r3" in regs and wrote_r3 and not regs["r3"].startswith("__CALLRET__"):
        ret = regs["r3"]  # a call's result falls through in r3 either way: `void f(void) { g(); }`
    # a call whose result is returned becomes `return f(...)`; one whose result feeds later code
    # becomes a temporary; the rest are statements
    used_ret = {i for i in range(len(calls)) if any(f"__CALLRET__{i}" in st for st in stmts if not st.startswith(f"__CALL__{i}(")) or ret == f"__CALLRET__{i}"}
    structs: List[str] = []
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
    if temps:
        body = temps + body
    if locals_:
        body = [f"struct {name}_{g} *{ln};" for ln, g in locals_.items()] + body
    # an address stored or passed is a pointer: cast, so u32 fields and parameters accept it
    body = [b if b.startswith("p_") else re.sub(r"= (&[A-Za-z_]\w*(?:\[0\])?);", r"= (u32)\1;", b) for b in body]
    body = [re.sub(r"(\(|, )(&[A-Za-z_]\w*(?:\[0\])?)(?=[,)])", r"\1(u32)\2", b) for b in body]
    fnames = {s for s, e in externs.items() if e.startswith("extern void ") and e.endswith("(void);")}
    for f in fnames:
        body = [re.sub(rf"(= |\(|, ){re.escape(f)}(?=[,;)])", rf"\1(u32){f}", b) for b in body]
    if ret is not None:
        body.append(f"return {ret};")
    def peephole(b: str) -> str:
        m = re.fullmatch(r"(\S.*?) = \((\S.*?) ([+-]) (\d+)\);", b)
        if m and m.group(1) == m.group(2):
            k = int(m.group(4))
            if k == 1:
                return f"{m.group(1)}{'++' if m.group(3) == '+' else '--'};"
            return f"{m.group(1)} {m.group(3)}= {k};"
        return b
    body = [peephole(b) for b in body]
    rtype_c = "void"
    if any(b.startswith("return ") for b in body):
        rtype_c = rtype.get("r3", "u32")
    if rtype_c == "void":
        body = [b.replace("return __RET__;", "return;") for b in body]
    else:
        # r3 at an early return holds the first parameter unless something wrote it before
        body = [b.replace("return __RET__;", f"return {'arg0' if params and params[0] == 'r3' else '0'};") for b in body]
    if "f1" in regs and any(a and a[0] == "f1" for mn, a in ins if mn != "blr") and not any(b.startswith("return ") for b in body):
        body.append(f"return {regs['f1']};"); rtype_c = rtype.get("f1", "f32")
        if rtype_c in ("s8", "s16"): rtype_c = "s32"
    # parameters and struct parameters
    decl_params = []
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
    def struct_text(sname: str, offs: Dict[int, str]) -> str:
        lines = [f"struct {sname} {{"]
        cur = 0
        for o in sorted(offs):
            if o > cur:
                lines.append(f"    u8 pad_{cur:X}[0x{o - cur:X}];")
            w = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "f32": 4, "f64": 8}[offs[o]]
            lines.append(f"    {offs[o]} unk_{o:X};")
            cur = o + w
        lines.append("};")
        return "\n".join(lines)
    for g, offs in gfields.items():
        sname = f"{name}_{g}"
        structs.append(struct_text(sname, offs))
        externs[g] = f"extern struct {sname} {g};"
    for g, offs in pfields.items():
        sname = f"{name}_{g}_T"
        structs.append(struct_text(sname, offs))
        externs[g] = re.sub(r"^extern \S+ ", f"extern struct {sname} *", externs[g]) if g in externs else f"extern struct {sname} *{g};"
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
    linkfail = {s for (s,) in db.execute("select symbol from attempts a where id = (select max(id) from attempts b where b.symbol = a.symbol) and outcome = 'link-mismatch'")}
    lifted = []
    for s, m, size in rows:
        if s in linkfail:
            continue  # matched the object and failed the link before: the same body fails again
        name = s.split(":", 1)[1] if ":" in s else s
        try:
            t = lift(p, m, name)
        except Exception:
            t = None
        if t:
            lifted.append((s, size, t))
    out_dir = STATE_DIR / "lift"; out_dir.mkdir(exist_ok=True)

    # one mwcc run per module over every lifted body, one cheap score each, the full check only
    # for the ones that score 100 (pool rows, data sections)
    results = []
    by_mod: Dict[str, list] = {}
    for s, size, t in lifted:
        by_mod.setdefault(s.split(":")[0] if ":" in s else p.resolve(s).module, []).append((s, size, t))
    for mod, items in by_mod.items():
        srcs = []
        for s, size, t in items:
            src = out_dir / (s.replace(":", "__") + ".c"); src.write_text(t); srcs.append(src)
        objs = oracle.compile_many(p, mod, srcs, out_dir / "obj" / mod)
        for (s, size, t), src in zip(items, srcs):
            o = objs.get(src)
            sym = p.resolve(s)
            target = p.target_object_for(sym) if sym else None
            if o is None or target is None:
                results.append((s, size, t, False, -1)); continue
            ok_, pct = oracle.function_score(p, sym.name, target, o)
            if ok_:
                r = oracle.check(p, s, 4, source=src)
                ok_ = r.ok and (r.matched or r.matched_pool) and oracle.unit_fully_matches(r) is None
            results.append((s, size, t, ok_, pct))
    # near misses get the deterministic fixup (type flips, symbol substitutions, layout edits)
    from . import fixup
    fixed = 0
    for idx, (s, size, t, ok, pct) in enumerate(results):
        if ok or pct < 90:
            continue
        fx = fixup.try_fix(p, s, t, budget_s=10.0)
        if fx.get("matched") and fx.get("body"):
            results[idx] = (s, size, fx["body"], True, 100.0); fixed += 1
    matched = [(s, size, t) for s, size, t, ok, _ in results if ok]
    submitted, failed = [], []
    if submit:
        for s, size, t in matched:
            work = p.work_path(s if ":" in s else p.key(p.resolve(s)))
            work.parent.mkdir(parents=True, exist_ok=True)
            work.write_text(t)
            r = api.submit(p, s, agent="lift", message="lifted from the disassembly (fzgx trivial)", harness="fzgx", model="lift")
            (submitted if r.get("ok") else failed).append(s if r.get("ok") else (s, str(r.get("error"))[:80]))
    return {"candidates": len(rows), "lifted": len(lifted), "matched": len(matched), "fixed": fixed, "bytes": sum(x[1] for x in matched),
            "submitted": len(submitted), "failed": failed[:10],
            "near": sorted(((s, round(pc, 1)) for s, _, _, ok, pc in results if not ok and pc >= 80), key=lambda x: -x[1])[:10]}
