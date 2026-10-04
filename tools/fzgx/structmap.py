"""Retail struct fingerprint: which field offsets each base register actually touches.

Written after profiling showed the unmatched population is dominated by large functions
whose seeded bodies have the wrong struct layouts. `fn_8005DCEC` (1980 words, 1.04%)
for example stores 15 distinct `stw` offsets through r3 and 8 packed `sth` offsets through
r9, while our object manages one `stw` and no `sth` at all -- the layout, not the
codegen, is what is wrong.

Reading this list is the first thing to do with a badly-scoring function: every offset
here is a field the struct definition has to account for, and a region retail writes
densely (0x1408-0x14f0 on that function) is usually one embedded struct we have flattened
or mis-sized. Reads the retail object only, so it is fast and needs no candidate body.
"""

import re
from pathlib import Path
from typing import Dict, List, Optional

from capstone import CS_ARCH_PPC, CS_MODE_32, CS_MODE_BIG_ENDIAN, Cs

from . import oracle

# `lwz r0, 0x1628(r31)` / `sth r3, -0x4(r30)`. Bases and signed displacements only.
_MEM = re.compile(r'^(l\w\w|s\w\w)\s+r?\d*,\s*(-?0x[0-9a-f]+|-?\d+)\(r(\d+)\)')
_LOAD, _STORE = 'l', 's'
# r1 is the stack, not a struct pointer; these never describe a layout.
_SKIP_BASES = frozenset({1})


def _decode(words: Optional[List[int]]) -> List[str]:
    if not words:
        return []
    md = Cs(CS_ARCH_PPC, CS_MODE_32 | CS_MODE_BIG_ENDIAN)
    md.skipdata = True  # undecodable words become .byte rows instead of truncating the list
    return [f'{i.mnemonic} {i.op_str}'.strip() for i in
            md.disasm(b''.join(w.to_bytes(4, 'big') for w in words), 0)]


def _accesses(texts: List[str]) -> Dict[int, Dict[str, set]]:
    """base register -> opcode -> displacements. """
    out: Dict[int, Dict[str, set]] = {}
    for t in texts:
        m = _MEM.match(t)
        if not m:
            continue
        op, off, base = m.group(1), int(m.group(2), 0), int(m.group(3))
        if base in _SKIP_BASES:
            continue
        out.setdefault(base, {}).setdefault(op, set()).add(off)
    return out


def fingerprint(p, symbol: str) -> Dict[str, object]:
    sym = p.resolve(symbol)
    words = oracle.words(Path(p.target_object_for(sym)), sym.name)
    acc = _accesses(_decode(words))
    bases = {}
    for base, ops in sorted(acc.items()):
        flat = sorted({o for v in ops.values() for o in v})
        bases['r%d' % base] = {
            'distinct': len(flat),
            'min': flat[0],
            'max': flat[-1],
            'ops': {op: sorted(ops[op]) for op in sorted(ops)},
        }
    return {'ok': True, 'symbol': symbol, 'module': sym.module, 'words': len(words),
            'bases': bases}


def _hex(v: int) -> str:
    return hex(v) if v >= 0 else '-0x%x' % -v


def format_fingerprint(r: Dict[str, object]) -> str:
    if not r.get('ok'):
        return 'STRUCTMAP FAILED: %s' % r.get('error')
    out = ["STRUCTMAP %s (%s)  retail %d words" % (r['symbol'], r['module'], r['words'])]
    if not r['bases']:
        out.append('  no struct-relative memory access found')
        return '\n'.join(out)
    for name, b in r['bases'].items():
        out.append('  base %s: %d distinct offsets (%s..%s)'
                   % (name, b['distinct'], _hex(b['min']), _hex(b['max'])))
        for op, offs in b['ops'].items():
            shown = ' '.join(_hex(o) for o in offs[:16])
            more = ' +%d more' % (len(offs) - 16) if len(offs) > 16 else ''
            kind = 'store' if op.startswith(_STORE) else 'load'
            out.append('     %-4s %-5s %2d: %s%s' % (op, kind, len(offs), shown, more))
    return '\n'.join(out)
