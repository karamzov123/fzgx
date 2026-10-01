#!/usr/bin/env python3
"""Which instruction-scheduling flag does each unit's retail object want?

Two plateaus on 2026-09-07 were not code problems, they were flag problems, and
both were visible in the retail objects before a single candidate was compiled:

  * `EXIGetState` held at 64.167% across 22 different C spellings. The unit
    wanted `-opt noschedule`; with it, three functions went byte-exact in one
    compile.
  * `InitializeUART` held at 92.25% and `__OSEnableBarnacle` at 94.31% -- in
    the same unit, wanting the OPPOSITE flag. They were a second SDK
    translation unit the carve had glued on, and splitting them out took both
    halves to fully exact.

The fingerprint is the epilogue. MWCC's default `-O4,p` schedule emits

    lmw   rN, X(r1)
    lwz   r0, Y(r1)      <- LR reload after the register restore

and with the scheduler off it emits the reload FIRST:

    lwz   r0, Y(r1)
    lmw   rN, X(r1)

Both orders appear in retail, so the retail object states the flag directly.
This reads that, per function, and reports:

  noschedule   the unit's functions were built with the scheduler off
  scheduled    ... with the default schedule
  SPLIT?       both appear, in two contiguous address ranges -- the carve
               merged two translation units; split at the boundary printed
  MIXED        both appear, interleaved: not a split, look closer

Only functions with an `lmw` epilogue carry the fingerprint, so a unit can be
`unknown` simply for having none. Absence of evidence is reported as such.

    python3 tools/natc_schedprobe.py                 # every unit
    python3 tools/natc_schedprobe.py --unit main/dolphin/os/EXIBios
    python3 tools/natc_schedprobe.py --disagreeing   # only units whose
                                                     # configured flag differs
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OBJDUMP = ROOT / 'build' / 'binutils' / 'powerpc-eabi-objdump'
FLAG = '-opt noschedule'


def classify(obj):
    """{function: 'noschedule' | 'scheduled'} for every lmw-epilogue function."""
    out = {}
    order = []
    try:
        text = subprocess.run([str(OBJDUMP), '-d', '--section=.text', str(obj)],
                              capture_output=True, text=True,
                              timeout=120).stdout
    except (OSError, subprocess.SubprocessError):
        return out, order
    cur = None
    recent = []
    for line in text.splitlines():
        m = re.match(r'^([0-9a-f]+) <([^>]+)>:', line)
        if m:
            cur = m.group(2)
            order.append((int(m.group(1), 16), cur))
            recent = []
            continue
        if cur is None:
            continue
        body = line.split('\t')[-1].strip()
        if re.match(r'lmw\s+r\d+,\s*-?\d+\(r1\)', body):
            # the LR reload is the `lwz r0, N(r1)` nearest this restore
            before = any(re.match(r'lwz\s+r0,\s*-?\d+\(r1\)', b)
                         for b in recent[-2:])
            out[cur] = 'noschedule' if before else 'scheduled'
        recent.append(body)
    return out, order


def configured_flags():
    """Units whose configure.py Object() carries -opt noschedule."""
    text = (ROOT / 'configure.py').read_text()
    flagged = set()
    for m in re.finditer(r'Object\(\s*\w+\s*,\s*"([^"]+)"(.*?)\)\s*,',
                         text, re.S):
        if FLAG in m.group(2):
            flagged.add(m.group(1))
    return flagged


def boundary(order, verdicts):
    """If the two verdicts split the unit in two, the FUNCTION they split at.

    Reported by name, not address: these are object-relative offsets, so an
    address here would be a different number from the one in splits.txt and
    inviting a mis-split is worse than saying less.
    """
    seq = [(addr, name, verdicts[name]) for addr, name in sorted(order)
           if name in verdicts]
    if len(seq) < 2:
        return None
    changes = [i for i in range(1, len(seq)) if seq[i][2] != seq[i - 1][2]]
    if len(changes) != 1:
        return None
    i = changes[0]
    return f'{seq[i - 1][1]} | {seq[i][1]}'  


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--unit')
    ap.add_argument('--disagreeing', action='store_true',
                    help='only units whose evidence differs from configure.py')
    args = ap.parse_args()

    cfg = json.loads((ROOT / 'objdiff.json').read_text())
    flagged = configured_flags()
    rows = []
    for unit in cfg['units']:
        if args.unit and unit['name'] != args.unit:
            continue
        target = ROOT / unit.get('target_path', '')
        if not target.is_file():
            continue
        verdicts, order = classify(target)
        if not verdicts:
            continue
        kinds = set(verdicts.values())
        n_ns = sum(1 for v in verdicts.values() if v == 'noschedule')
        src = (unit.get('metadata') or {}).get('source_path') or ''
        has_flag = src.removeprefix('src/') in flagged
        if len(kinds) == 1:
            verdict = kinds.pop()
            note = ''
        else:
            at = boundary(order, verdicts)
            verdict = 'SPLIT?' if at else 'MIXED'
            note = f'{verdicts[at.split(" | ")[0]]} ends at {at}' if at else ''
        wants = verdict == 'noschedule'
        disagrees = (verdict in ('noschedule', 'scheduled')
                     and wants != has_flag) or verdict == 'SPLIT?'
        if args.disagreeing and not disagrees:
            continue
        rows.append((disagrees, unit['name'], verdict, n_ns, len(verdicts),
                     has_flag, note))

    if not rows:
        print('natc_schedprobe: nothing to report.')
        return 0
    print(f"{'unit':42s} {'verdict':11s} {'ns/total':>9s} {'flagged':>8s}  note")
    for disagrees, name, verdict, n_ns, total, has_flag, note in sorted(
            rows, key=lambda r: (not r[0], r[1])):
        mark = '!' if disagrees else ' '
        print(f'{mark}{name:41s} {verdict:11s} {n_ns:4d}/{total:<4d} '
              f'{"yes" if has_flag else "no":>8s}  {note}')
    bad = sum(1 for r in rows if r[0])
    print(f'\n{len(rows)} unit(s) carry the fingerprint; {bad} disagree with '
          'configure.py.')
    print('An lmw-less unit carries no evidence and is not listed. A verdict '
          'is evidence about\nthe retail object, not a promise that the flag '
          'makes a candidate match.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
