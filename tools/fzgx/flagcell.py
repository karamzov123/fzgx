"""Name what a compiler flag or pragma actually did, by diffing our own two objects.

A candidate's score against retail says *whether* a setting helped. It never says
*what the setting did*, so a helpful flag cannot be reproduced in source and an
unhelpful one cannot be dismissed on evidence. This compiles one body twice under two
settings and reports the instruction-level difference between the two objects we
produced ourselves -- no retail in the comparison, so every row is an attributable
effect of exactly that setting.

Method and the "diff your two objects" step are from `SFA-Decomp`'s
`source_shape_levers.md` flag-cell probe. The setting names are this project's
(`optimizer_pragmas`), so the probe covers the whole pragma cell.
"""

from __future__ import annotations

import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple

from capstone import Cs, CS_ARCH_PPC, CS_MODE_32, CS_MODE_BIG_ENDIAN

from . import oracle
from .project import Project

PRAGMAS = ('peephole', 'opt_propagation', 'opt_common_subs', 'opt_lifetimes',
           'opt_dead_assignments', 'opt_strength_reduction', 'opt_loop_invariants',
           'scheduling')

# The effects that look like "the compiler did something unrelated" when seen only as a
# score change: constants materialised early, and the prologue's save/copy sequence
# reordered. Both are what findings/110 and the -O4,p scheduler actually do.
EFFECTS = (
    (re.compile(r'^\s*(li|lis)\s'), 'constant materialised'),
    (re.compile(r'^\s*stw\s+r(1[3-9]|2\d|3[01]),'), 'callee-saved store (prologue)'),
    (re.compile(r'^\s*mr\s+r(1[3-9]|2\d|3[01]),'), 'callee-saved copy (prologue)'),
    (re.compile(r'^\s*(b|beq|bne|blt|bgt|ble|bge|bc|bclr)\s'), 'branch moved'),
)


def _disasm(words: Optional[List[int]]) -> List[str]:
    if not words:
        return []
    md = Cs(CS_ARCH_PPC, CS_MODE_32 | CS_MODE_BIG_ENDIAN)
    return [f'{i.mnemonic} {i.op_str}'.strip() for i in
            md.disasm(b''.join(w.to_bytes(4, 'big') for w in words), 0)]


def apply_pragma(body: str, name: str, option: str, value: str) -> str:
    """`option` set to `value` for `name`'s span only, preserving nested pragma state.

    Mirrors `fixup_evidence.optimizer_pragmas`: an existing directive for the option is
    flipped in place so a nested state survives; otherwise a scoped pair is inserted.
    """
    from .fixup_evidence import _function_span
    span = _function_span(body, name)
    if span is None:
        return body
    start = body.rfind('\n', 0, span[0]) + 1
    stack = []
    for m in re.finditer(rf'(?m)^[ \t]*#pragma\s+{re.escape(option)}\s+(on|off|reset)\b', body[:span[0]]):
        if m[1] == 'reset':
            if stack:
                stack.pop()
        else:
            stack.append(m)
    if stack:
        pragma = stack[-1]
        want = 'on' if pragma[1] == 'off' else 'off'
        a, b = pragma.span(1)
        return body[:a] + want + body[b:]
    return (body[:start] + f'#pragma {option} {value}\n' + body[start:span[1]] +
            f'\n#pragma {option} reset\n' + body[span[1]:])


def _cell(body: str, name: str, mw: Optional[str], setting: str):
    """(source, mw, cflags) for one cell. A pragma name becomes a scoped pragma; anything
    else (e.g. `-O3`) is passed through as a cflag so non-pragma settings are probeable."""
    parts = setting.split()
    if parts and parts[0] in PRAGMAS:
        value = parts[1] if len(parts) > 1 else 'off'
        return apply_pragma(body, name, parts[0], value), mw, None
    return body, mw, setting


def probe(project: Project, symbol: str, body: str, a: Optional[str] = None,
          b: Optional[str] = None, mw: Optional[str] = None, max_rows: int = 40) -> Dict[str, object]:
    """Compile `body` under setting `a` and setting `b`; report what changed between them.

    The row list is the A/B difference -- the mechanism, not the score. Retail's words
    and both word scores are included so the direction of each cell is visible.
    """
    sym = project.resolve(symbol)
    if sym is None:
        return dict(ok=False, error='unknown or ambiguous symbol (use module:name)')
    target = project.target_object_for(sym)
    if target is None:
        return dict(ok=False, error='no retail object for this symbol')
    tw = oracle.words(Path(target), sym.name)
    a = a if a is not None else 'as-is'
    b = b if b is not None else 'scheduling off'

    cells: Dict[str, Tuple[Optional[List[int]], str]] = {}
    for tag, setting in (('a', a), ('b', b)):
        if setting == 'as-is':
            text, cell_mw, cell_flags = body, mw, None
        else:
            text, cell_mw, cell_flags = _cell(body, sym.name, mw, setting)
        stem = Path(project.build_dir) / 'flagcell' / (sym.name.replace(':', '_') + f'.{tag}')
        stem.parent.mkdir(parents=True, exist_ok=True)
        src, obj = stem.with_suffix('.c'), stem.with_suffix('.o')
        src.write_text(text)
        cp = oracle.compile_source(project, sym.module, src, obj, cell_mw, cell_flags)
        cells[tag] = (oracle.words(obj, sym.name) if cp.returncode == 0 and obj.exists() else None, setting)

    aw, bw = cells['a'][0], cells['b'][0]
    if aw is None or bw is None:
        bad = [t for t in ('a', 'b') if cells[t][0] is None]
        return dict(ok=False, error=f'cell(s) failed to compile: {",".join(bad)}',
                    settings={t: cells[t][1] for t in cells})
    A, B = _disasm(aw), _disasm(bw)
    rows = [dict(row=i, a=A[i] if i < len(A) else '', b=B[i] if i < len(B) else '')
            for i in range(max(len(A), len(B)))
            if (A[i] if i < len(A) else '') != (B[i] if i < len(B) else '')]
    effects = []
    for r in rows:
        for pattern, label in EFFECTS:
            if pattern.match(r['b']) and not pattern.match(r['a']):
                if label not in effects:
                    effects.append(label)
                break
    return dict(ok=True, symbol=sym.name, module=sym.module, retail_words=len(tw or []),
                a=dict(setting=a, words=len(aw), percent=oracle.word_score(tw, aw)[0] if tw else -1.0),
                b=dict(setting=b, words=len(bw), percent=oracle.word_score(tw, bw)[0] if tw else -1.0),
                length_changed=len(aw) != len(bw), differing_rows=len(rows),
                named_effects=effects, rows=rows[:max_rows], truncated=len(rows) > max_rows)


def format_probe(r: Dict[str, object]) -> str:
    if not r.get('ok'):
        return 'FLAGCELL FAILED: %s' % r.get('error')
    out = [f"FLAGCELL {r['symbol']} ({r['module']})  retail {r['retail_words']} words",
           f"  A {r['a']['setting']:<26} {r['a']['words']:>4} words  {r['a']['percent']:7.3f}%",
           f"  B {r['b']['setting']:<26} {r['b']['words']:>4} words  {r['b']['percent']:7.3f}%"]
    if r['length_changed']:
        out.append('  LENGTH CHANGED: this setting added or removed instructions, not just reordered')
    if not r['differing_rows']:
        out.append('  the two cells compiled identically: this setting had no effect here')
        return '\n'.join(out)
    if r['named_effects']:
        out.append('  named effects: ' + ', '.join(r['named_effects']))
    out.append(f"  {r['differing_rows']} differing rows (A|B):")
    for row in r['rows']:
        out.append('  %4d  %-34s | %s' % (row['row'], row['a'][:34], row['b'][:34]))
    if r.get('truncated'):
        out.append('  ... truncated')
    return '\n'.join(out)

