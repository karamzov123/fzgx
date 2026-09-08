"""Mechanical C for straight-line functions: getters, setters, wrappers and short call-free
bodies, lifted from the retail disassembly. No model involved; the oracle verifies every
candidate and `fzgx trivial` submits the ones that match.

The lifter is a register-level symbolic evaluator over one basic block: constants, address
materialisation (lis/addi, @ha/@l), loads and stores by symbol or by base+offset (a struct
parameter), the common integer arithmetic, one call. Anything else makes it give up.
"""

from __future__ import annotations

import json
import re
from typing import Dict, List, Optional, Tuple

from .project import Project

WIDTH = {"lwz": 4, "lhz": 2, "lha": 2, "lbz": 1, "lfs": 4, "lfd": 8, "stw": 4, "sth": 2, "stb": 1, "stfs": 4, "stfd": 8}
LOAD_T = {"lwz": "u32", "lhz": "u16", "lha": "s16", "lbz": "u8", "lfs": "f32", "lfd": "f64",
          "lwzu": "u32", "lhzu": "u16", "lbzu": "u8", "lfsu": "f32", "lfdu": "f64"}
STORE_T = {"stw": "u32", "sth": "u16", "stb": "u8", "stfs": "f32", "stfd": "f64"}
LABELS: List[Dict[str, int]] = [{}]
ARITY_HINT: List[Dict[str, int]] = [{}]        # callee -> widest integer-argument count (second run)
ARITY_SEEN: List[Dict[str, List[int]]] = [{}]  # filled by a run: what each site passed
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
            continue  # a forward jump: the end of a then-block or a loop entry (checked when lifted)
        if mn == "bdnz":
            continue  # a counted loop: the struct-copy idiom explains it, or _lift gives up
        m = re.fullmatch(r"b(\w+)", mn)
        if not (m and m.group(1) in COND and a and a[-1].startswith(".L_")):
            return None
    if len(ins) > 160:
        return None
    try:
        ARITY_HINT[0] = {}; ARITY_SEEN[0] = {}
        text = _lift(p, module, name, ins)
        # a callee whose sites disagree on the argument count gets the widest prototype, and
        # every narrower site passes what its argument register held (the source did)
        hint = {c: max(v) for c, v in ARITY_SEEN[0].items() if len(set(v)) > 1}
        if hint:
            ARITY_HINT[0] = hint
            try:
                text = _lift(p, module, name, ins)
            finally:
                ARITY_HINT[0] = {}
        return text
    except Give:
        return None


def skeleton(p: Project, module: str, name: str) -> Optional[str]:
    """What the lifter recovers before it gives up: declarations, layouts, locals, the leading
    statements, and a marker for what is left. For the agent's context when no full draft exists."""
    fa = p.function_asm(module).get(name)
    if fa is None:
        return None
    ins: List[Tuple[str, List[str]]] = []
    labels: Dict[str, int] = {}
    for ln in fa.asm:
        t = ln.strip()
        if t.startswith(".L_") and t.endswith(":"):
            labels[t[:-1]] = len(ins); continue
        m = LINE_RE.match(t)
        if m:
            ins.append((m.group(1), [a.strip() for a in m.group(2).split(",")] if m.group(2) else []))
    if not ins:
        return None
    LABELS[0] = labels
    try:
        text = _lift(p, module, name, ins, partial=True)
    except Give:
        return None
    except Exception:
        return None
    return text


def lift_variants(p: Project, module: str, name: str) -> List[str]:
    """Every spelling worth checking: with stack locals, MWCC's frame layout depends on the
    declaration order, so both plausible orders are candidates (the oracle picks)."""
    first = lift(p, module, name)
    if first is None:
        return []
    out = [first]
    fa = p.function_asm(module).get(name)
    ins: List[Tuple[str, List[str]]] = []
    for ln in fa.asm:
        t = ln.strip()
        if t.startswith(".L_"):
            continue
        m = LINE_RE.match(t)
        if m:
            ins.append((m.group(1), [a.strip() for a in m.group(2).split(",")] if m.group(2) else []))
    # the spellings the oracle must choose between: frame layout order, and whether a value
    # passed several times to one call goes through a temporary
    options = [("grouped", True), ("reverse", False), ("grouped", False)] if "/* frame */" in first else [("reverse", False)]
    for layout, site_temps in options:
        try:
            alt = _lift(p, module, name, ins, layout=layout, site_temps=site_temps)
        except Give:
            continue
        if alt and alt not in out:
            out.append(alt)
    return out


def _lift(p: Project, module: str, name: str, ins, layout: str = "reverse", site_temps: bool = True, partial: bool = False) -> Optional[str]:
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

    def indexed_field(b: str, o: int, t: str) -> Optional[str]:
        """`(base + (idx << k))` / `(base + (idx * n))` with a displacement is an array field of the
        struct the base points to: `base->unk_off[idx]` (elements of the access width), or, when
        the stride is wider, an array of element structs `base->unk_off[idx].unk_0`. MWCC then
        emits the retail `add; load disp(r)` instead of an indexed load."""
        m = re.fullmatch(r"\((arg\d+|p_[A-Za-z_]\w*) \+ \((.+?) (<<|\*) (\d+)\)\)", b) or \
            re.fullmatch(r"\(\((.+?) (<<|\*) (\d+)\) \+ (arg\d+|p_[A-Za-z_]\w*)\)", b)
        if not m:
            return None
        g = m.groups()
        if g[0].startswith(("arg", "p_")):
            bexpr, idx, op, n = g
        else:
            idx, op, n, bexpr = g
        stride = (1 << int(n)) if op == "<<" else int(n)
        w = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "f32": 4, "f64": 8}[t]
        if stride < w or o < 0:
            return None
        if bexpr.startswith("arg"):
            k = int(bexpr[3:])
            if k >= len(params):
                return None
            key = params[k]
            tab = fields.setdefault(key, {})
        elif bexpr in locals_:
            tab = gfields.setdefault(locals_[bexpr], {})
        else:
            return None
        if stride == w:
            tab[o] = f"arr:{t}:{stride}"
            return f"{bexpr}->unk_{o:X}[{idx}]"
        tab[o] = f"arr:struct {name}_E{stride}_{t}:{stride}"
        elem_structs[(stride, t)] = f"struct {name}_E{stride}_{t} {{ {t} unk_0; u8 pad_{w:X}[0x{stride - w:X}]; }};"
        return f"{bexpr}->unk_{o:X}[{idx}].unk_0"

    elem_structs: Dict[Tuple[int, str], str] = {}

    hi: Dict[str, str] = {}  # register holding sym@ha
    labels = LABELS[0]
    frame_size = 0
    for mn_, a_ in ins:
        if mn_ == "stwu" and a_ and a_[0] == "r1":
            m_ = re.match(r"^(-?0x[0-9a-f]+|-?\d+)\(r1\)$", a_[1])
            if m_:
                frame_size = -int(m_.group(1), 0)
            break
    saved_slots: set = set()
    seen_written: set = set()
    for mn_, a_ in ins:
        # a callee-saved register's prologue save: stored before the function writes it
        if mn_ in ("stw", "stfd", "psq_st") and a_ and (SAVE_RE.match(a_[0]) or re.fullmatch(r"f(1[4-9]|2\d|3[01])", a_[0])) and "(r1)" in a_[1] and a_[0] not in seen_written:
            m_ = re.match(r"^(-?0x[0-9a-f]+|-?\d+)\(r1\)$", a_[1])
            if m_:
                saved_slots.add(int(m_.group(1), 0))
        if a_ and mn_ not in STORE_T and not mn_.startswith(("st", "cmp", "b")) and mn_ not in ("mtlr", "mtspr", "mtctr"):
            seen_written.add(a_[0])
    slocals: Dict[int, Dict[str, object]] = {}  # frame offset -> {"w": width or 0 (address only), "t": type}
    # struct copies through the count register: recognised up front so their address setup
    # (dst-4 / src-4) is not mistaken for locals or pointer arithmetic
    copies: Dict[int, Tuple[int, str, str, int]] = {}  # index of `li rN, K` -> (K, rD, rS, bdnz index)
    for j_, (mn_, a_) in enumerate(ins):
        if mn_ == "mtctr" and a_:
            li_ = next((x for x in range(j_ - 1, max(-1, j_ - 6), -1) if ins[x][0] == "li" and ins[x][1][0] == a_[0]), None)
            body_ = ins[j_ + 1:j_ + 6]
            if li_ is not None and len(body_) == 5 and [m for m, _ in body_] == ["lwz", "lwzu", "stw", "stwu", "bdnz"]:
                (l1, l2, s1, s2, _) = body_
                rS = re.search(r"\((r\d+)\)$", l1[1][1]).group(1); rD = re.search(r"\((r\d+)\)$", s1[1][1]).group(1)
                if l1[1][1].startswith("0x4(") and l2[1][1].startswith("0x8(") and s1[1][1].startswith("0x4(") and s2[1][1].startswith("0x8("):
                    copies[li_] = (_imm(ins[li_][1][1]), rD, rS, j_ + 5)
    struct_syms = {sym_of(a_[2]) for mn_, a_ in ins if mn_ == "addi" and len(a_) == 3 and sym_of(a_[2]) and not a_[1] == "r1"}
    copy_dst_locals: Dict[int, int] = {}  # frame offset of a copied-into local -> size
    for li_, (K, rD, rS, end_) in copies.items():
        for x in range(li_, end_):
            if ins[x][0] == "addi" and ins[x][1][0] == rD and ins[x][1][1] == "r1":
                copy_dst_locals[_imm(ins[x][1][2]) + 4] = 8 * K
    taken = sorted({_imm(a_[2]) for mn_, a_ in ins if mn_ == "addi" and len(a_) == 3 and a_[1] == "r1"
                    and _imm(a_[2]) + 4 not in copy_dst_locals} | set(copy_dst_locals))
    top_of_locals = min(saved_slots) if saved_slots else frame_size

    def owner(off: int):
        """The address-taken local whose extent covers `off`, and the offset inside it."""
        for i_, t_ in enumerate(taken):
            end = taken[i_ + 1] if i_ + 1 < len(taken) else top_of_locals
            if t_ <= off < end:
                return t_, off - t_
        return None

    def local_at(off: int, w: int, t: str) -> str:
        own = owner(off)
        if own is not None:
            base_off, inner = own
            ent = slocals.setdefault(base_off, {"w": 0, "t": "u8", "addr": True, "elems": {}})
            if w and not ent["w"]:
                ent["w"] = w; ent["t"] = t
            ent.setdefault("elems", {})[inner] = (w, t)
            ew = ent["w"] or w or 1
            i_ = taken.index(base_off)
            extent = (taken[i_ + 1] if i_ + 1 < len(taken) else top_of_locals) - base_off
            if extent <= ew:
                return f"loc_{base_off:X}"  # a scalar whose address is taken
            if extent >= 16:  # struct-wrapped (see the declarations)
                return f"loc_{base_off:X}.a[{inner // ew}]" if inner % ew == 0 else f"*({t} *)((u8 *)&loc_{base_off:X} + {inner})"
            return f"loc_{base_off:X}[{inner // ew}]" if inner % ew == 0 else f"*({t} *)((u8 *)loc_{base_off:X} + {inner})"
        ent = slocals.setdefault(off, {"w": 0, "t": "u8", "addr": False, "elems": {}})
        if w and not ent["w"]:
            ent["w"] = w; ent["t"] = t
        return f"loc_{off:X}"
    temps: List[str] = []
    written_since_call: set = set()
    variadic_next = [False]
    copy_types: set = set()
    magic_div: Dict[str, Tuple[str, int]] = {}  # register holding mulhwu(x, magic) -> (x, magic)
    pending_div: Dict[int, Tuple[str, str]] = {}  # index of the idiom's last instruction -> (register, quotient expression)
    conv_slots: Dict[int, Tuple[str, Optional[str]]] = {}  # stack slot -> int/float conversion in progress

    def divisor_of(magic: int, post_shift: int, add: bool) -> Optional[int]:
        """The d for which MWCC's magicu(d) is (magic, add, post_shift): search small divisors."""
        for d in range(2, 1 << 20):
            # Hacker's Delight magicu
            nc = (1 << 32) - 1 - (((1 << 32) - d) % d)
            p_ = 31; q1 = (1 << 31) // nc; r1 = (1 << 31) - q1 * nc; q2 = ((1 << 31) - 1) // d; r2 = ((1 << 31) - 1) - q2 * d
            a_ = False
            while True:
                p_ += 1
                if r1 >= nc - r1:
                    q1 = 2 * q1 + 1; r1 = 2 * r1 - nc
                else:
                    q1 = 2 * q1; r1 = 2 * r1
                if r2 + 1 >= d - r2:
                    if q2 >= (1 << 31) - 1: a_ = True
                    q2 = 2 * q2 + 1; r2 = 2 * r2 + 1 - d
                else:
                    if q2 >= (1 << 31): a_ = True
                    q2 = 2 * q2; r2 = 2 * r2 + 1
                delta = d - 1 - r2
                if not (p_ < 64 and (q1 < delta or (q1 == delta and r1 == 0))):
                    break
            M = (q2 + 1) & 0xFFFFFFFF; sh = p_ - 32
            if M == magic and a_ == add and sh == post_shift:
                return d
            if d > 4096 and d % 1000 != 0:
                continue
        return None
    pending_ptr: Dict[str, str] = {}

    class _Stmts(list):
        def append(self, st):
            for ln_, asg in list(pending_ptr.items()):
                if re.search(rf"\b{re.escape(ln_)}\b", st) and st != asg:
                    del pending_ptr[ln_]; super().append(asg)
            # an address assigned to an integer temporary is cast (temporaries are integers unless
            # they hold a struct pointer, see the declarations)
            m_ = re.fullmatch(r"(v\d+) = (\(\(u8 \*\).*|\(u8 \*\).*|&[A-Za-z_]\w*.*);", st)
            if m_:
                st = f"{m_.group(1)} = (u32){m_.group(2)};"
            super().append(st)
    stmts = _Stmts(stmts)
    fnames_seen: set = set()
    array_locals: set = set()

    def reads(idx: int, r: str) -> bool:
        """Is register r read at instruction idx (before being written there)?"""
        mn_, a_ = ins[idx]
        if not a_:
            return False
        srcs = a_[1:] if mn_ not in STORE_T and not mn_.startswith(("st", "cmp")) else a_
        return any(re.search(rf"\b{r}\b", x) for x in srcs)

    def read_later(idx: int, r: str) -> bool:
        """Is r read after instruction idx before being written again (a call reads r3..r10)?"""
        for j in range(idx + 1, len(ins)):
            mn_, a_ = ins[j]
            if reads(j, r):
                return True
            if mn_ == "bl":
                if re.fullmatch(r"r([3-9]|10)|f([1-8])", r):
                    return True
                if re.fullmatch(r"r([0-9]|1[0-2])|f(\d|1[0-3])", r):
                    return False
            if mn_ == "blr":
                return r in ("r3", "f1")
            if a_ and a_[0] == r and mn_ not in STORE_T and not mn_.startswith(("st", "cmp")):
                return False
        return False

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
    gave_at: Optional[int] = None
    temps_written: List[Tuple[str, int]] = []
    carried: Dict[str, str] = {}     # register -> local name while inside a loop region
    loop_regions: List[Tuple[int, int, int]] = []  # (body_start, test_start, backbranch_index)
    for j_, (mn_, a_) in enumerate(ins):
        if mn_ == "b" and a_ and a_[-1].startswith(".L_"):
            tst = labels.get(a_[-1], -1)
            if tst > j_:
                # the back branch: the first conditional branch at/after the test that targets j_+1
                for k_ in range(tst, len(ins)):
                    m_ = re.fullmatch(r"b(\w+)", ins[k_][0])
                    if m_ and m_.group(1) in COND and ins[k_][1] and labels.get(ins[k_][1][-1]) == j_ + 1:
                        loop_regions.append((j_ + 1, tst, k_)); break
    loop_by_entry = {b - 1: (b, t, k) for b, t, k in loop_regions}
    loop_end_by_index = {k: (b, t, k) for b, t, k in loop_regions}
    in_loop: List[Tuple[int, int, int]] = []
    for i, (mn, a) in enumerate(ins):
        try:
            # a value used more than once (before its register is redefined) lives in a local: the
            # compiler would otherwise recompute or reschedule the expression at each use
            if i > 0:
                pm, pa = ins[i - 1]
                pd = pa[0] if pa and pm not in STORE_T and not pm.startswith(("st", "cmp", "b")) and pm not in ("mtlr", "mtspr", "bl") else None
                if pd and pd in regs and pd not in carried and not re.fullmatch(r"[A-Za-z_]\w*|-?\d+|0x[0-9A-Fa-f]+|&[A-Za-z_]\w*", regs[pd]) and not regs[pd].startswith(("(struct ", "__CALLRET__", "((u8 *)&", "&")):
                    uses = 0
                    for x in range(i, len(ins)):
                        if reads(x, pd) or (ins[x][0] == "bl" and re.fullmatch(r"r([3-9]|10)|f[1-8]", pd)):
                            uses += 1
                        if ins[x][0] == "blr" and pd in ("r3", "f1"):
                            uses += 1  # returned
                        if ins[x][0] == "bl" and re.fullmatch(r"r([0-9]|1[0-2])|f([0-9]|1[0-3])", pd):
                            break
                        if ins[x][1] and ins[x][1][0] == pd and ins[x][0] not in STORE_T and not ins[x][0].startswith(("st", "cmp")):
                            break
                    if uses >= 2:
                        tn = f"v{len(temps)}"; temps.append(f"{rtype.get(pd, 'u32')} {tn};")
                        stmts.append(f"{tn} = {regs[pd]};"); regs[pd] = tn
            # a carried register that now holds a new expression: materialise the assignment
            for r_, tn in list(carried.items()):
                if regs.get(r_) is None:
                    regs[r_] = tn  # cleared by a call: the local still holds the value
                elif regs.get(r_) != tn:
                    e_ = regs.get(r_)
                    if e_.startswith(("((u8 *)", "(u8 *)", "&", "(struct ")):
                        e_ = f"(u32){e_}"  # a register reused for an address: the local is an integer
                    stmts.append(f"{tn} = {e_};"); regs[r_] = tn
            if a and mn not in STORE_T and not mn.startswith(("st", "cmp", "b")) and mn not in ("mtlr", "mtspr"):
                temps_written.append((a[0], i)); written_since_call.add(a[0])
            if mn == "bl":
                pass  # cleared after the call is processed (see the bl branch)
            # the previous instruction wrote a callee-saved register with a computed value that a
            # call will intervene before its use: the source kept it in a local
            if i > 0:
                pm, pa = ins[i - 1]
                pd = pa[0] if pa and pm not in STORE_T and not pm.startswith(("st", "cmp", "b")) and pm not in ("mtlr", "mtspr") else None
                if pd and SAVE_RE.match(pd) and pd not in carried and pd in regs and not re.fullmatch(r"[A-Za-z_]\w*|-?\d+|0x[0-9A-Fa-f]+|&[A-Za-z_]\w*", regs[pd]) and not regs[pd].startswith("(struct "):
                    call_before_use = False
                    for x in range(i, len(ins)):
                        if ins[x][0] == "bl":
                            call_before_use = True; break
                        if reads(x, pd) or (ins[x][1] and ins[x][1][0] == pd and ins[x][0] not in STORE_T):
                            break
                    if call_before_use:
                        tn = f"v{len(temps)}"; temps.append(f"{rtype.get(pd, 'u32')} {tn};")
                        stmts.append(f"{tn} = {regs[pd]};"); regs[pd] = tn
            while open_ifs and open_ifs[-1][0] == i:
                stmts.append(open_ifs.pop()[1])
            if i in pending_div:
                d_, e_ = pending_div.pop(i)
                regs[d_] = e_; rtype[d_] = "u32"
            if i in copies:
                K, rD, rS, end_ = copies[i]
                size = 8 * K
                dst = src = None
                for x in range(i + 1, end_):
                    mn_x, a_x = ins[x]
                    if mn_x == "addi" and a_x[0] == rD and a_x[1] == "r1":
                        off_ = _imm(a_x[2]) + 4
                        slocals.setdefault(off_, {"w": 4, "t": "u32", "addr": True, "elems": {}})
                        dst = f"loc_{off_:X}"
                    elif mn_x in ("subi", "addi") and a_x[0] == rD:
                        base_e = use(a_x[1]); k_ = _imm(a_x[2]) * (-1 if mn_x == "subi" else 1) + 4
                        dst = f"*(struct {name}_Copy{size} *)((u8 *){base_e} + {k_})" if k_ else f"*(struct {name}_Copy{size} *){base_e}"
                    elif mn_x in ("subi", "addi") and a_x[0] == rS:
                        base_e = use(a_x[1]); k_ = _imm(a_x[2]) * (-1 if mn_x == "subi" else 1) + 4
                        src = f"*(struct {name}_Copy{size} *)((u8 *){base_e} + {k_})" if k_ else f"*(struct {name}_Copy{size} *){base_e}"
                if dst is None or src is None:
                    raise Give()
                copy_types.add(size)
                stmts.append(f"{dst} = {src};")
                for x in range(i, end_ + 1):
                    skip.add(x)
                for r_ in (rD, rS, "r0", "r3"):
                    regs.pop(r_, None)
                continue
            if i in skip:
                continue
            if i in loop_by_entry:
                b_, t_, k_ = loop_by_entry[i]
                # loop-carried registers: written inside [b_, k_] and read inside before written, or read by the test
                def writes(x):
                    mn_x, a_x = ins[x]
                    return a_x[0] if a_x and mn_x not in STORE_T and not mn_x.startswith(("st", "cmp", "b")) and mn_x not in ("mtlr", "mtspr") else None
                def reads_of(x):
                    mn_x, a_x = ins[x]
                    srcs = a_x[1:] if mn_x not in STORE_T and not mn_x.startswith(("st", "cmp", "b")) else a_x
                    return set(re.findall(r"\b([rf]\d+)\b", " ".join(srcs)))
                written_in = {writes(x) for x in range(b_, k_ + 1) if writes(x)}
                live_in, seen_w = set(), set()
                for x in range(b_, t_):
                    live_in |= (reads_of(x) - seen_w)
                    w_ = writes(x)
                    if w_:
                        seen_w.add(w_)
                test_reads = set()
                for x in range(t_, k_ + 1):
                    test_reads |= reads_of(x)
                for r_ in sorted((live_in | test_reads) & written_in):
                    if r_ in ("r1", "r0") or r_ in carried:
                        continue
                    tn = f"v{len(temps)}"
                    init = regs.get(r_)
                    if init is None:
                        init = use(r_) if re.fullmatch(r"r([3-9]|10)|f[1-8]", r_) else "0"
                    temps.append(f"{rtype.get(r_, 'u32')} {tn};")
                    stmts.append(f"{tn} = {init};")
                    regs[r_] = tn; carried[r_] = tn
                # the test, evaluated on the pre-loop state, gives the condition
                saved_regs, saved_rtype, saved_len = dict(regs), dict(rtype), len(stmts)
                cond_expr = None
                for x in range(t_, k_):
                    mn_x, a_x = ins[x]
                    if mn_x in ("cmpwi", "cmpw", "cmplwi", "cmplw"):
                        lhs = use(a_x[0]); rhs = str(_imm(a_x[1])) if mn_x.endswith("i") else use(a_x[1])
                        uns = mn_x.startswith("cmpl")
                        def typed2(e: str, reg: str) -> str:
                            tt = rtype.get(reg, "u32")
                            if uns:
                                return e if tt in ("u32", "u16", "u8") else f"(u32){e}"
                            return e if tt in ("s32", "s16", "s8") else f"(s32){e}"
                        lhs = typed2(lhs, a_x[0])
                        if not mn_x.endswith("i"):
                            rhs = typed2(rhs, a_x[1])
                        cond_expr = (lhs, rhs)
                    elif mn_x == "extsb":
                        regs[a_x[0]] = f"(s8){use(a_x[1])}"; rtype[a_x[0]] = "s8"
                    elif mn_x == "extsh":
                        regs[a_x[0]] = f"(s16){use(a_x[1])}"; rtype[a_x[0]] = "s16"
                    elif mn_x in LOAD_T and a_x and not a_x[1].endswith("(r1)"):
                        raise Give()  # a load in the test: keep the region out of the lifter for now
                    else:
                        raise Give()
                if cond_expr is None:
                    raise Give()
                m_ = re.fullmatch(r"b(\w+)", ins[k_][0])
                op = COND[m_.group(1)]
                stmts.append(f"while ({cond_expr[0]} {op} {cond_expr[1]}) {{")
                regs, rtype = saved_regs, saved_rtype
                # the body runs next; the test instructions and the back branch are consumed
                for x in range(t_, k_ + 1):
                    skip.add(x)
                open_ifs.append((k_ + 1, "}"))
                open_ifs.sort(key=lambda x: -x[0])
                in_loop.append((b_, t_, k_))
                continue
            if mn == "b":
                raise Give()  # an unconditional jump that no if/else or loop explained
            if mn == "blr":
                if i == len(ins) - 1 or not any(True for _ in ins[i + 1:]):
                    break
                # an early return: the value r3 holds here, then the lift goes on at the label
                # that follows (the fallthrough code the branch skipped over)
                if "r3" in regs and any(a_ and a_[0] == "r3" for mn_, a_ in ins[:i] if mn_ not in ("stw", "sth", "stb", "stfs", "stfd", "cmpwi", "cmpw", "cmplwi", "cmplw")):
                    stmts.append(f"return {regs['r3']};")
                    regs.pop("r3", None)
                elif "f1" in regs and any(a_ and a_[0] == "f1" for mn_, a_ in ins[:i]):
                    stmts.append(f"return {regs['f1']};")
                    regs.pop("f1", None)
                else:
                    stmts.append("return __RET__;")
                continue
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
                    raise Give()  # a back edge no loop region explained
                op = COND[m.group(1)]
                inv = {"==": "!=", "!=": "==", "<": ">=", ">": "<=", "<=": ">", ">=": "<"}[op]  # branch taken = skip
                l, r_, uns = cond
                if ins[tgt][0] == "blr" or tgt == len(ins) - 1:
                    stmts.append(f"if ({l} {op} {r_}) {{ return __RET__; }}")
                else:
                    # every register the region writes and code after it may read is a local: its
                    # writes become statements inside the branches, reads after use the local
                    region_end = tgt
                    pm0, pa0 = ins[tgt - 1]
                    if pm0 == "b" and pa0 and pa0[-1].startswith(".L_") and labels.get(pa0[-1], -1) > tgt:
                        region_end = labels[pa0[-1]]
                    written = []
                    for x in range(i + 1, region_end):
                        mn_x, a_x = ins[x]
                        if a_x and mn_x not in STORE_T and not mn_x.startswith(("st", "cmp", "b")) and mn_x not in ("mtlr", "mtspr", "mtctr"):
                            if a_x[0] not in written:
                                written.append(a_x[0])
                    for rw in written:
                        if rw in ("r0", "r1") or rw in carried or not re.fullmatch(r"r([3-9]|1\d|2\d|3[01])|f([1-9]|1\d|2\d|3[01])", rw):
                            continue
                        read_after = any(reads(x, rw) or (ins[x][0] == "bl" and re.fullmatch(r"r([3-9]|10)|f[1-8]", rw)) or (ins[x][0] == "blr" and rw in ("r3", "f1"))
                                         for x in range(region_end, len(ins)))
                        read_inside_first = False
                        for x in range(i + 1, region_end):
                            if reads(x, rw):
                                read_inside_first = True; break
                            if ins[x][1] and ins[x][1][0] == rw and ins[x][0] not in STORE_T and not ins[x][0].startswith(("st", "cmp", "b")):
                                break
                        if not (read_after or read_inside_first):
                            continue
                        init0 = regs.get(rw)
                        if init0 is not None and (re.fullmatch(r"&[A-Za-z_]\w*", init0) or init0.startswith("((u8 *)&") or init0.startswith("(struct ")):
                            continue  # MWCC rematerialises addresses: no local
                        tn = f"v{len(temps)}"
                        init = regs.get(rw)
                        if init is None and re.fullmatch(r"r([3-9]|10)|f[1-8]", rw):
                            init = use(rw)
                        temps.append(f"{rtype.get(rw, 'u32')} {tn};")
                        if init is not None:
                            stmts.append(f"{tn} = {init};")
                        regs[rw] = tn; carried[rw] = tn
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
                open_ifs.sort(key=lambda x: -x[0])  # smallest index on top: every closer pops at its index
                continue
            lr_slot = f"0x{frame_size + 4:x}(r1)" if frame_size else None
            slot_ = None
            if a and len(a) > 1 and a[1].endswith("(r1)"):
                m_s = re.match(r"^(-?0x[0-9a-f]+|-?\d+)", a[1])
                slot_ = int(m_s.group(1), 0) if m_s else None
            if mn in ("stwu", "mflr", "mtlr") or (mn in ("stw", "lwz") and a and ((a[0] == "r0" and a[1] == lr_slot) or (SAVE_RE.match(a[0]) and slot_ in saved_slots))) or (mn == "addi" and a and a[0] == "r1"):
                frame = True
                continue
            if mn == "addi" and len(a) == 3 and a[1] == "r1":
                off_ = _imm(a[2])
                local_at(off_, 0, "u8"); slocals[off_]["addr"] = True
                regs[a[0]] = f"&loc_{off_:X}"; rtype[a[0]] = "void *"; frame = True
                continue
            if mn in ("stw", "stfd") and a and a[1].endswith("(r1)"):
                off_ = _imm(a[1][:-4])
                e_ = regs.get(a[0], "")
                if mn == "stw" and e_ in ("0x43300000", "1127219200"):
                    conv_slots[off_] = ("hi", None); frame = True; continue
                if mn == "stw" and off_ - 4 in conv_slots and conv_slots[off_ - 4][0] == "hi":
                    conv_slots[off_ - 4] = ("pair", e_); frame = True; continue
                if mn == "stfd" and e_.startswith("__FCTIWZ__("):
                    conv_slots[off_] = ("fctiwz", e_[len("__FCTIWZ__("):-1]); frame = True; continue
            if mn == "lfd" and a and a[1].endswith("(r1)") and _imm(a[1][:-4]) in conv_slots and conv_slots[_imm(a[1][:-4])][0] == "pair":
                x = conv_slots[_imm(a[1][:-4])][1]
                m_ = re.fullmatch(r"__XORIS__\((.+), 32768\)", x)
                regs[a[0]] = f"__I2D__({m_.group(1)}, signed)" if m_ else f"__I2D__({x}, unsigned)"
                rtype[a[0]] = "f64"; frame = True; continue
            if mn == "lwz" and a and a[1].endswith("(r1)") and _imm(a[1][:-4]) - 4 in conv_slots and conv_slots[_imm(a[1][:-4]) - 4][0] == "fctiwz":
                regs[a[0]] = f"(s32){conv_slots[_imm(a[1][:-4]) - 4][1]}"; rtype[a[0]] = "s32"; frame = True; continue
            if mn in LOAD_T and a and a[1].endswith("(r1)"):
                off_ = _imm(a[1][:-4]); t = LOAD_T[mn]
                if off_ in saved_slots or off_ >= frame_size:
                    frame = True; continue
                regs[a[0]] = local_at(off_, WIDTH[mn], t); rtype[a[0]] = t; frame = True
                continue
            if mn in STORE_T and a and a[1].endswith("(r1)"):
                off_ = _imm(a[1][:-4]); t = STORE_T[mn]
                if off_ in saved_slots or off_ >= frame_size or a[0] == "r0" and off_ > frame_size:
                    frame = True; continue
                stmts.append(f"{local_at(off_, WIDTH[mn], t)} = {use(a[0])};"); frame = True
                continue
            if mn in ("stfd", "lfd", "psq_st", "psq_l") and a and re.fullmatch(r"f(1[4-9]|2\d|3[01])", a[0]) and slot_ in saved_slots:
                frame = True
                continue  # callee-saved float registers
            if mn in ("crclr", "crset") or mn == "nop":
                if mn == "crclr":
                    variadic_next[0] = True  # `crclr cr1eq`: the callee is variadic (no float varargs)
                continue
            if mn == "lis" and sym_of(a[1]):
                hi[a[0]] = sym_of(a[1]); regs.pop(a[0], None); continue
            if mn == "lis":
                regs[a[0]] = f"0x{(_imm(a[1]) & 0xFFFF) << 16:X}"; rtype[a[0]] = "u32"; continue
            if mn == "addi" and sym_of(a[2]) and a[1] in hi:
                s = sym_of(a[2]); declare(s, "u32")
                sd = lookup(s)
                so = sym_off(a[2])
                if sd is not None and sd.kind == "function":
                    regs[a[0]] = s; fnames_seen.add(s)
                elif so:
                    regs[a[0]] = f"((u8 *)&{s} + {so})"
                elif SAVE_RE.match(a[0]):
                    # kept in a callee-saved register: the source held it in a local pointer
                    ln = f"p_{s}"
                    locals_[ln] = s
                    declare(s, "struct", far_ref=True); gfields.setdefault(s, {})
                    # the cast is what keeps the address in the register across calls: MWCC
                    # rematerialises a plain `&sym` after each call, but not a cast of it.
                    # The assignment is emitted right before the first statement that uses it.
                    stmts.append(f"{ln} = (struct {name}_{s} *)&{s};")  # eager: retail places it early
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
                    if so or s in struct_syms:
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
                    fb = field_base(b, base) if o >= 0 else None
                    if fb is None:
                        ax = indexed_field(b, o, t)
                        if ax is not None and not mn.endswith("u"):
                            regs[a[0]] = ax; rtype[a[0]] = t
                            if reused_after_store(i, a[0], a[1]):
                                tn = f"v{len(temps)}"; temps.append(f"{t} {tn};")
                                stmts.append(f"{tn} = {regs[a[0]]};"); regs[a[0]] = tn
                            continue
                        # a computed address (array element, pointer arithmetic): a plain typed access
                        regs[a[0]] = f"*({t} *)((u8 *){b} + {o})"; rtype[a[0]] = t
                        if mn.endswith("u"):
                            regs[base] = f"((u8 *){b} + {o})"
                        continue
                    kind, key, k = fb
                    if kind == "param":
                        fields.setdefault(key, {})[o] = t; regs[a[0]] = f"{b}->unk_{o:X}"
                    elif kind == "global" and b in locals_:
                        gfields.setdefault(key, {})[o] = t; regs[a[0]] = f"{b}->unk_{o:X}"
                    elif kind == "global" and key in ptr_globals and o + k == 0 and t == "u32":
                        regs[a[0]] = key  # the pointer variable itself, read through its address
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
                # a value loaded before this store and read after it was a local in the source
                # (the swap idiom: `old = p->x; p->x = v; return old;`); left pending, the read
                # would be emitted after the store and see the new value
                for r_ in list(regs):
                    e_ = regs[r_]
                    if r_ == a[0] or not e_ or not re.search(r"\*\(|->|\.unk_|(?<![\w])[A-Za-z_]\w*\[", e_) or re.fullmatch(r"[A-Za-z_]\w*", e_):
                        continue
                    if not read_later(i, r_):
                        continue
                    tn = f"v{len(temps)}"; temps.append(f"{rtype.get(r_, 'u32')} {tn};")
                    stmts.append(f"{tn} = {e_};"); regs[r_] = tn
                val = use(a[0])
                if off.endswith("@l") and base in hi:
                    s = hi[base]; so = sym_off(off)
                    if so or s in struct_syms:
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
                    fb = field_base(b, base) if o >= 0 else None
                    if fb is None:
                        ax = indexed_field(b, o, t)
                        if ax is not None:
                            stmts.append(f"{ax} = {val};"); continue
                        stmts.append(f"*({t} *)((u8 *){b} + {o}) = {val};"); continue
                    kind, key, k = fb
                    if kind == "param":
                        fields.setdefault(key, {})[o] = t; stmts.append(f"{b}->unk_{o:X} = {val};")
                    elif kind == "global" and b in locals_:
                        gfields.setdefault(key, {})[o] = t; stmts.append(f"{b}->unk_{o:X} = {val};")
                    elif kind == "global" and key in ptr_globals and o + k == 0 and t == "u32":
                        stmts.append(f"{key} = {val};")
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
                elif mn == "srwi":
                    src_e = regs.get(a[1], "")
                    m_ = re.fullmatch(r"\(\(\((.+) - __MULHU__\((.+), (\d+)\)\) >> 1\) \+ __MULHU__\(\2, \3\)\)", src_e) if src_e else None
                    if m_ and m_.group(1) == m_.group(2):  # the add form: x - q >> 1 + q, then >> (s-1)
                        dv = divisor_of(int(m_.group(3)), _imm(a[2]) + 1, True)
                        if dv is None:
                            raise Give()
                        regs[d] = f"({m_.group(1)} / {dv})"; rtype[d] = "u32"
                    else:
                        m2 = re.fullmatch(r"__MULHU__\((.+), (\d+)\)", src_e) if src_e else None
                        if m2:
                            dv = divisor_of(int(m2.group(2)), _imm(a[2]), False)
                            if dv is None:
                                raise Give()
                            regs[d] = f"({m2.group(1)} / {dv})"; rtype[d] = "u32"
                        else:
                            regs[d] = f"((u32){use(a[1])} >> {_imm(a[2])})"; rtype[d] = "u32"
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
            if mn.endswith(".") and mn[:-1] in ("extrwi", "rlwinm", "andi", "extsb", "extsh", "clrlwi", "subic", "addic", "and", "or", "subf", "add", "neg", "srawi", "cntlzw", "xor", "mulli", "slwi", "srwi"):
                # record form: the result is also compared with zero for the branch that follows
                base_mn = mn[:-1]
                ins_i = (base_mn, a)
                handled = False
                # evaluate through the plain op by recursion on a one-instruction list is awkward:
                # replicate the few cases inline
                if base_mn == "extrwi":
                    n, b = _imm(a[2]), _imm(a[3]); regs[a[0]] = f"(({use(a[1])} >> {32 - b - n}) & 0x{(1 << n) - 1:X})"; rtype[a[0]] = "u32"; handled = True
                elif base_mn == "andi":
                    regs[a[0]] = f"({use(a[1])} & {_imm(a[2])})"; rtype[a[0]] = "u32"; handled = True
                elif base_mn == "extsb":
                    regs[a[0]] = f"(s8){use(a[1])}"; rtype[a[0]] = "s8"; handled = True
                elif base_mn == "extsh":
                    regs[a[0]] = f"(s16){use(a[1])}"; rtype[a[0]] = "s16"; handled = True
                elif base_mn == "clrlwi":
                    n = 32 - _imm(a[2]); regs[a[0]] = f"({use(a[1])} & 0x{(1 << n) - 1:X})"; rtype[a[0]] = "u32"; handled = True
                elif base_mn in ("subic", "addic"):
                    k = _imm(a[2]); regs[a[0]] = f"({use(a[1])} {'-' if base_mn == 'subic' else '+'} {k})"; rtype[a[0]] = rtype.get(a[1], "s32"); handled = True
                elif base_mn == "and":
                    regs[a[0]] = f"({use(a[1])} & {use(a[2])})"; rtype[a[0]] = "u32"; handled = True
                elif base_mn == "or":
                    regs[a[0]] = f"({use(a[1])} | {use(a[2])})"; rtype[a[0]] = "u32"; handled = True
                elif base_mn == "subf":
                    regs[a[0]] = f"({use(a[2])} - {use(a[1])})"; rtype[a[0]] = "s32"; handled = True
                elif base_mn == "add":
                    regs[a[0]] = f"({use(a[1])} + {use(a[2])})"; rtype[a[0]] = "s32"; handled = True
                elif base_mn == "neg":
                    regs[a[0]] = f"(-{use(a[1])})"; rtype[a[0]] = "s32"; handled = True
                elif base_mn == "rlwinm":
                    sh, mb, me = _imm(a[2]), _imm(a[3]), _imm(a[4])
                    if sh == 0 and mb == 0: regs[a[0]] = f"({use(a[1])} & 0x{(0xFFFFFFFF << (31 - me)) & 0xFFFFFFFF:X})"
                    elif sh == 0 and me == 31: regs[a[0]] = f"({use(a[1])} & 0x{(1 << (32 - mb)) - 1:X})"
                    else: raise Give()
                    rtype[a[0]] = "u32"; handled = True
                if not handled:
                    raise Give()
                signed = rtype.get(a[0]) in ("s8", "s16", "s32")
                cond = (regs[a[0]], "0", not signed); continue
            if mn in ("lwzx", "lhzx", "lbzx", "lfsx", "lhax"):
                t = {"lwzx": "u32", "lhzx": "u16", "lbzx": "u8", "lfsx": "f32", "lhax": "s16"}[mn]
                b = use(a[1]); i2 = use(a[2])
                regs[a[0]] = f"*({t} *)((u8 *){b} + {i2})"; rtype[a[0]] = t; continue
            if mn in ("stwx", "sthx", "stbx", "stfsx"):
                t = {"stwx": "u32", "sthx": "u16", "stbx": "u8", "stfsx": "f32"}[mn]
                stmts.append(f"*({t} *)((u8 *){use(a[1])} + {use(a[2])}) = {use(a[0])};"); continue
            if mn in ("fmuls", "fadds", "fsubs", "fdivs", "fmul", "fadd", "fsub", "fdiv"):
                op = {"fmuls": "*", "fadds": "+", "fsubs": "-", "fdivs": "/", "fmul": "*", "fadd": "+", "fsub": "-", "fdiv": "/"}[mn]
                t = "f32" if mn.endswith("s") else "f64"
                m_ = re.fullmatch(r"__I2D__\((.+), (signed|unsigned)\)", regs.get(a[1], ""))
                if m_ and op == "-":
                    cast = "(s32)" if m_.group(2) == "signed" else "(u32)"
                    regs[a[0]] = f"({t}){cast}{m_.group(1)}"; rtype[a[0]] = t
                    written_since_call.discard(a[2]); regs.pop(a[2], None)  # the constant, not an argument
                    continue
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
            if mn == "andc":
                regs[a[0]] = f"({use(a[1])} & ~{use(a[2])})"; rtype[a[0]] = "u32"; continue
            if mn == "mulhwu":
                # unsigned division by a constant: q = mulhu(x, m) then the fix-up sequence; the
                # divisor is recovered from the magic number (Hacker's Delight magicu). The whole
                # idiom is matched here by looking ahead, so its steps never leak into expressions.
                m_expr = regs.get(a[1]); x_reg = a[2]; x_expr = use(x_reg)
                if m_expr is None:
                    raise Give()
                try:
                    magic = int(eval(m_expr, {"__builtins__": {}})) & 0xFFFFFFFF if re.fullmatch(r"[0-9x\s()+\-*A-Fa-f]+", m_expr) else None
                except Exception:
                    magic = None
                if magic is None:
                    raise Give()
                q = a[0]
                # the fix-up steps may be interleaved with unrelated instructions: find them in order
                def find(start, pred):
                    for x in range(start, min(len(ins), start + 12)):
                        if pred(ins[x][0], ins[x][1]):
                            return x
                    return None
                j1 = find(i + 1, lambda m_, a_: m_ == "subf" and len(a_) == 3 and a_[1] == q and a_[2] == x_reg)
                if j1 is not None:
                    t1 = ins[j1][1][0]
                    j2 = find(j1 + 1, lambda m_, a_: m_ == "srwi" and a_[1] == t1 and _imm(a_[2]) == 1)
                    if j2 is not None:
                        t2 = ins[j2][1][0]
                        j3 = find(j2 + 1, lambda m_, a_: m_ == "add" and set(a_[1:]) == {t2, q})
                        if j3 is not None:
                            t3 = ins[j3][1][0]
                            j4 = find(j3 + 1, lambda m_, a_: m_ == "srwi" and a_[1] == t3)
                            if j4 is not None:
                                dv = divisor_of(magic, _imm(ins[j4][1][2]) + 1, True)
                                if dv is None:
                                    raise Give()
                                for x in (j1, j2, j3, j4):
                                    skip.add(x)
                                pending_div[j4] = (ins[j4][1][0], f"({x_expr} / {dv})")
                                continue
                j1 = find(i + 1, lambda m_, a_: m_ == "srwi" and a_[1] == q)
                if j1 is not None:
                    dv = divisor_of(magic, _imm(ins[j1][1][2]), False)
                    if dv is None:
                        raise Give()
                    skip.add(j1)
                    pending_div[j1] = (ins[j1][1][0], f"({x_expr} / {dv})")
                    continue
                raise Give()
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
            if mn == "xoris":
                regs[a[0]] = f"__XORIS__({use(a[1])}, {_imm(a[2])})"; rtype[a[0]] = "u32"; continue
            if mn == "fctiwz":
                regs[a[0]] = f"__FCTIWZ__({use(a[1])})"; rtype[a[0]] = "f64"; continue
            if mn == "cntlzw":
                regs[a[0]] = f"__cntlzw({use(a[1])})"; rtype[a[0]] = "u32"; continue
            if mn == "bl":
                callee = a[0]
                if lookup(callee) is None:
                    raise Give()
                # arguments: r3..rN where N is the highest argument register set here; a lower
                # register never written is a parameter of ours passed straight through
                # an argument register counts only if this function wrote it since the last call
                # (a stale value from earlier code is not an argument), or passes a parameter through
                # an argument is a register written since the last call whose value is not consumed
                # by the caller before the call (a register read after its last write was a temporary)
                def is_arg(rk: str) -> bool:
                    if rk not in regs:
                        return False
                    if rk in params and regs[rk] == f"arg{params.index(rk)}":
                        return True
                    if rk not in written_since_call:
                        return False
                    last_w = max((x for x, (m_, a_) in enumerate(ins[:i]) if a_ and a_[0] == rk and m_ not in STORE_T and not m_.startswith(("st", "cmp", "b"))), default=None)
                    if last_w is None:
                        return True
                    return not any(reads(x, rk) for x in range(last_w + 1, i))
                set_regs = [k for k in range(3, 11) if is_arg(f"r{k}")]
                top = max(set_regs) if set_regs else 2
                # registers below the lowest temporary this function used are parameters passed
                # straight through to the callee: a temporary in r6 with r3..r5 untouched means
                # the source wrote callee(a, b, c)
                written = [int(x[1:]) for x, _ in temps_written if re.fullmatch(r"r([3-9]|10)", x)]
                temp_low = min((w for w in written if f"r{w}" not in params), default=None)
                if temp_low is not None and temp_low - 1 > top:
                    top = temp_low - 1
                want = ARITY_HINT[0].get(callee)
                if want is not None and 2 + want > top and all(f"r{k}" in regs for k in range(top + 1, 3 + want)):
                    top = 2 + want
                ARITY_SEEN[0].setdefault(callee, []).append(top - 2)
                fset = [k for k in range(1, 9) if f"f{k}" in regs and (f"f{k}" in written_since_call or f"f{k}" in params)]
                ftop = max(fset) if fset else 0
                fargs = [use(f"f{k}") for k in range(1, ftop + 1)]
                ptypes_ = []
                for k in range(3, top + 1):
                    e = regs.get(f"r{k}", "")
                    stack_addr = e.startswith("&loc_") or (e.startswith("loc_") and e[4:].split("[")[0].isalnum() and slocals.get(int(e[4:].split("[")[0], 16), {}).get("addr"))
                    other_addr = rtype.get(f"r{k}") == "void *" or e.startswith(("&", "(u8 *)", "((u8 *)", "(struct ")) or e in fnames_seen or e in ptr_globals
                    if stack_addr:
                        ptypes_.append("void *")  # `&x` with a pointer parameter is recomputed per call
                    else:
                        if other_addr and not e.startswith("(u32)"):
                            regs[f"r{k}"] = f"(u32){e}"
                        ptypes_.append("u32")
                for k in range(1, ftop + 1):
                    ptypes_.append(rtype.get(f"f{k}", "f32"))
                args = [use(f"r{k}") for k in range(3, top + 1)] + fargs  # after the casts
                seen_args: Dict[str, int] = {}
                for e in args:
                    seen_args[e] = seen_args.get(e, 0) + 1
                for e, n_ in seen_args.items():
                    if site_temps and n_ >= 2 and not re.fullmatch(r"[A-Za-z_]\w*|-?\d+|0x[0-9A-Fa-f]+", e):
                        tn = f"v{len(temps)}"
                        tt = next((rtype.get(r_) for r_, ex in regs.items() if ex == e), "u32") or "u32"
                        temps.append(f"{tt} {tn};"); stmts.append(f"{tn} = {e};")
                        args = [tn if x == e else x for x in args]
                        for r_ in list(regs):
                            if regs[r_] == e:
                                regs[r_] = tn
                calls.append(callee)
                written_since_call.clear()
                if variadic_next[0]:
                    variadic_next[0] = False
                    proto = f"extern u32 {callee}({ptypes_[0] if ptypes_ else 'void *'}, ...);"
                    externs[callee] = proto
                else:
                    proto = f"extern u32 {callee}({', '.join(ptypes_) or 'void'});"
                    prev = externs.get(callee)
                    if prev is None or prev.endswith("(void);") or (prev.count(",") < proto.count(",") and "..." not in prev):
                        externs[callee] = proto
                stmts.append(f"__CALL__{len(calls) - 1}({', '.join(args)});")
                for r in list(regs):
                    if re.fullmatch(r"r([0-9]|1[0-2])|f([0-9]|1[0-3])", r):
                        regs.pop(r, None)
                regs["r3"] = f"__CALLRET__{len(calls) - 1}"; rtype["r3"] = "u32"
                continue
            raise Give()

        except (Give, KeyError, IndexError, ValueError) as e:
            # a KeyError is the lifter losing track of a register: the same give-up, later
            if not partial or (not isinstance(e, Give) and i < 0):
                raise
            gave_at = i
            break
    if partial and gave_at is not None:
        left = len(ins) - gave_at
        nxt = "; ".join(f"{m_} {', '.join(a_)}" for m_, a_ in ins[gave_at:gave_at + 4])
        # what the registers held at that point, for the reader: not code, the values are partial
        held = [f"/* {r_} = {e_} */" for r_, e_ in sorted(regs.items()) if e_ and not re.fullmatch(r"(arg\d+|v\d+|t\d+|\d+|0x[0-9A-Fa-f]+)", e_)]
        stmts.extend(held[:12])
        stmts.append(f"/* NOT LIFTED from here: {left} instructions, starting `{nxt}` */")
        regs.clear()
    for ln_, asg in pending_ptr.items():
        if any(re.search(rf"\b{re.escape(ln_)}\b", e) for e in regs.values()):
            stmts.append(asg)
    pending_ptr.clear()
    # return value: whatever r3 holds at blr, when this function wrote r3 (an untouched first
    # parameter is not a return value; a parameter copied back after a call is)
    wrote_r3 = any(a and a[0] == "r3" and mn not in ("stw", "sth", "stb", "stfs", "stfd", "cmpwi", "cmpw", "cmplwi", "cmplw") for mn, a in ins)
    if partial and gave_at is not None:
        wrote_r3 = False  # the return value is beyond the give-up point
    if "r3" in regs and wrote_r3 and not regs["r3"].startswith("__CALLRET__"):
        ret = regs["r3"]  # a call's result falls through in r3 either way: `void f(void) { g(); }`
    # a call whose result is returned becomes `return f(...)`; one whose result feeds later code
    # becomes a temporary; the rest are statements
    used_ret = {i for i in range(len(calls)) if any(f"__CALLRET__{i}" in st for st in stmts if not st.startswith(f"__CALL__{i}(")) or (ret is not None and f"__CALLRET__{i}" in ret)}
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
    if slocals:
        offs = sorted(slocals)
        top = min(saved_slots) if saved_slots else frame_size
        decls = []
        for i_, off_ in enumerate(offs):
            ent = slocals[off_]
            nxt = offs[i_ + 1] if i_ + 1 < len(offs) else top
            size = max(nxt - off_, ent["w"] or 1)
            w = ent["w"] or 1
            if off_ in copy_dst_locals:
                decls.append((off_, f"struct {name}_Copy{copy_dst_locals[off_]} loc_{off_:X};", "struct")); continue
            if not ent["w"] and size in (1, 2, 4, 8):  # address only: a scalar of that size
                ent["t"] = {1: "u8", 2: "u16", 4: "u32", 8: "f64"}[size]; w = size
            elif not ent["w"] and size % 4 == 0:
                ent["t"] = "u32"; w = 4  # a u8 buffer would be 16-aligned by MWCC; a u32 array is not
            if size > w and size >= 16:
                decls.append((off_, f"struct {{ {ent['t']} a[{max(size // w, 1)}]; }} loc_{off_:X};", "struct")); continue
            if size > w:
                decls.append((off_, f"{ent['t']} loc_{off_:X}[{max(size // w, 1)}];", True))
            else:
                # a scalar whose address is taken: `&x` at each use, which MWCC does not hoist
                # (an array's decay it does, into a saved register)
                decls.append((off_, f"{ent['t']} loc_{off_:X};", False))
        if layout == "grouped":  # scalars first, arrays after, each group highest offset first
            ordered = [d for d in sorted(decls, key=lambda x: -x[0]) if d[2] is False] + [d for d in sorted(decls, key=lambda x: -x[0]) if d[2] is not False]
        else:  # highest frame offset declared first
            ordered = sorted(decls, key=lambda x: -x[0])
        body = [d[1] for d in ordered] + ["/* frame */"] + body
        # an address-taken array is passed as itself, not &array, and cast like every address
        arrays = {f"loc_{d[0]:X}" for d in decls if d[2] is True}  # struct-wrapped ones keep the &
        def fix_addr(b: str) -> str:
            return re.sub(r"&(loc_[0-9A-F]+)\b", lambda m: m.group(1) if m.group(1) in arrays else f"&{m.group(1)}", b)
        body = [fix_addr(b) if not b.startswith(("u8 loc", "u32 loc", "f32 loc", "s16 loc", "u16 loc", "s8 loc", "f64 loc", "struct {")) else b for b in body]
    if temps:
        # a temporary's type follows its first assignment: a pointer global's struct pointer,
        # `u8 *` for byte arithmetic and addresses, a struct pointer for the cast form
        ptr_types = {g: f"struct {name}_{g}_T *" for g in pfields}
        fixed_temps = []
        for tdecl in temps:
            tdecl = tdecl.replace("void * v", "u32 v")  # an address in an integer temporary
            m_ = re.fullmatch(r"(\S+) (v\d+);", tdecl)
            if m_:
                init_line = next((b for b in body if re.match(rf"{m_.group(2)} = ", b)), None)
                if init_line:
                    rhs = init_line[len(m_.group(2)) + 3:].rstrip(";")
                    if rhs in ptr_types:
                        tdecl = f"{ptr_types[rhs]}{m_.group(2)};"
                    else:
                        ms = re.match(r"\(struct (\w+) \*\)", rhs)
                        if ms:
                            tdecl = f"struct {ms.group(1)} *{m_.group(2)};"
            fixed_temps.append(tdecl)
        body = fixed_temps + body
    if locals_:
        body = [f"struct {name}_{g} *{ln};" for ln, g in locals_.items()] + body
    # an address stored or passed is a pointer: cast, so u32 fields and parameters accept it
    body = [b if b.startswith("p_") else re.sub(r"= (&[A-Za-z_]\w*(?:\[0\])?);", r"= (u32)\1;", b) for b in body]
    fnames = {s for s, e in externs.items() if e.startswith("extern void ") and e.endswith("(void);")}
    for f in fnames:
        body = [re.sub(rf"(= ){re.escape(f)}(?=;)", rf"\1(u32){f}", b) for b in body]
    if ret is not None:
        body.append(f"return {ret};")
    if not partial and (any(("__MULHU__" in b or "__I2D__" in b or "__XORIS__" in b or "__FCTIWZ__" in b) for b in body) or (ret and any(x in ret for x in ("__MULHU__", "__I2D__", "__XORIS__", "__FCTIWZ__")))):
        raise Give()
    def peephole(b: str) -> str:
        m = re.fullmatch(r"(\S.*?) = \((\S.*?) ([+-]) (\d+)\);", b)
        if m and m.group(1) == m.group(2):
            k = int(m.group(4))
            if k == 1:
                return f"{m.group(1)}{'++' if m.group(3) == '+' else '--'};"
            return f"{m.group(1)} {m.group(3)}= {k};"
        return b
    body = [peephole(b) for b in body]
    body = [re.sub(r"\((\S+) - \(\(\1 / (\d+)\) \* \2\)\)", r"(\1 % \2)", b) for b in body]
    rtype_c = "void"
    if any(b.startswith("return ") for b in body):
        rtype_c = rtype.get("r3", "u32")
        if rtype_c == "void *":
            rtype_c = "u32"
    if rtype_c == "void":
        body = [b.replace("return __RET__;", "return;") for b in body]
    else:
        # r3 at an early return holds the first parameter unless something wrote it before
        body = [b.replace("return __RET__;", f"return {'arg0' if params and params[0] == 'r3' else '0'};") for b in body]
    if "f1" in regs and any(a and a[0] == "f1" for mn, a in ins if mn != "blr") and not any(b.startswith("return ") for b in body):
        body.append(f"return {regs['f1']};"); rtype_c = rtype.get("f1", "f32")
        if rtype_c in ("s8", "s16"): rtype_c = "s32"
    def field_width(t: str) -> int:
        if t.startswith("arr:"):
            return int(t.rsplit(":", 1)[1])  # one element: enough for the padding that follows
        return {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "f32": 4, "f64": 8}[t]

    def field_decl(t: str, o: int) -> str:
        if t.startswith("arr:"):
            _, et, _ = t.split(":", 2) if t.count(":") == 2 else (None, t[4:t.rfind(":")], None)
            et = t[4:t.rfind(":")]
            return f"{et} unk_{o:X}[1];"
        return f"{t} unk_{o:X};"

    structs.extend(elem_structs.values())
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
                lines.append("    " + field_decl(offs[o], o))
                cur = o + field_width(offs[o])
            lines.append("};")
            structs.append("\n".join(lines))
            decl_params.append(f"struct {sname} *arg{i}")
        else:
            decl_params.append(f"{ptypes[r]} arg{i}")
    if calls and not frame and not partial:
        raise Give()
    def struct_text(sname: str, offs: Dict[int, str]) -> str:
        lines = [f"struct {sname} {{"]
        cur = 0
        for o in sorted(offs):
            if o > cur:
                lines.append(f"    u8 pad_{cur:X}[0x{o - cur:X}];")
            lines.append("    " + field_decl(offs[o], o))
            cur = o + field_width(offs[o])
        lines.append("};")
        return "\n".join(lines)
    empty_globals = [g for g, offs in gfields.items() if not offs]
    for size in sorted(copy_types):
        structs.append(f"struct {name}_Copy{size} {{ u32 a[{size // 4}]; }};")
    for g, offs in gfields.items():
        sname = f"{name}_{g}"
        if not offs:
            externs[g] = f"extern u8 {g}[];"
            continue
        structs.append(struct_text(sname, offs))
        externs[g] = f"extern struct {sname} {g};"
    for g, offs in pfields.items():
        sname = f"{name}_{g}_T"
        structs.append(struct_text(sname, offs))
        externs[g] = f"extern struct {sname} *{g};"
    for g in empty_globals:
        sname = f"{name}_{g}"
        body = [b.replace(f"struct {sname} *", "u8 *").replace(f"(struct {sname} *)", "(u8 *)") for b in body]
    ptr_names = [f"arg{i}" for i, r in enumerate(params) if r in fields] + [ln for ln in locals_ if ln.startswith("p_")] + list(pfields)
    decl_line = re.compile(r"^\s*(struct\s+\w+\s*\*+|[A-Za-z_]\w*\s*\*+|[A-Za-z_]\w*\s+)\s*[A-Za-z_]\w*(\[[^\]]*\])*;$")
    for pn in ptr_names:
        body = [re.sub(rf"(?<![\w>.*])({re.escape(pn)})\b(?!\s*->|\s*=\s*\(struct)", r"(u32)\1", b)
                if not (b.startswith(f"{pn} = ") or decl_line.match(b)) else b for b in body]
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
            variants = lift_variants(p, m, name)
        except Exception:
            variants = []
        for vi, t in enumerate(variants):
            lifted.append((s if vi == 0 else f"{s}#{vi}", size, t))
    out_dir = STATE_DIR / "lift"; out_dir.mkdir(exist_ok=True)

    # one mwcc run per module over every lifted body, one cheap score each, the full check only
    # for the ones that score 100 (pool rows, data sections)
    results = []
    by_mod: Dict[str, list] = {}
    for s, size, t in lifted:
        s0 = s.split("#")[0]
        by_mod.setdefault(s0.split(":")[0] if ":" in s0 else p.resolve(s0).module, []).append((s, size, t))
    for mod, items in by_mod.items():
        srcs = []
        for s, size, t in items:
            src = out_dir / (s.replace(":", "__").replace("#", "__v") + ".c"); src.write_text(t); srcs.append(src)
        objs = oracle.compile_many(p, mod, srcs, out_dir / "obj" / mod)
        for (s, size, t), src in zip(items, srcs):
            o = objs.get(src)
            s = s.split("#")[0]  # a layout variant of the same function
            sym = p.resolve(s)
            target = p.target_object_for(sym) if sym else None
            if o is None or target is None:
                results.append((s, size, t, False, -1)); continue
            tw_, ow_ = oracle.words(target, sym.name), oracle.words(o, sym.name)
            if not tw_ or not ow_:
                results.append((s, size, t, False, -1)); continue
            pct, bad_ = oracle.word_score(tw_, ow_)
            ok_ = not bad_ and len(ow_) == len(tw_)
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
    best: Dict[str, tuple] = {}
    for s, size, t, ok, pct in results:
        if s not in best or (ok, pct) > (best[s][3], best[s][4]):
            best[s] = (s, size, t, ok, pct)
    results = list(best.values())
    # the per-function best draft and its score, for the lab and the context (no rescoring later)
    scores_path = STATE_DIR / "lift" / "scores.json"
    try:
        prev = json.loads(scores_path.read_text()) if scores_path.exists() else {}
    except ValueError:
        prev = {}
    for s, size, t, ok, pct in results:
        prev[s] = {"percent": pct, "matched": bool(ok), "text": t}
    scores_path.write_text(json.dumps(prev))
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
