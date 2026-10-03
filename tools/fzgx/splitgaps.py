#!/usr/bin/env python3
"""Report `.text` split gaps: known functions that no split range covers.

An unmatched function with no split entry cannot be linked: carving a unit for it adds a
*second* copy of the function (the module grows and the byte count no longer matches retail),
or the REL step fails to resolve a symbol it expected. The per-object oracle still reports
100% for the carved body, because it diffs one object against the retail object and never
sees the module. That is why these look object-perfect and link-rejected with no obvious
cause; see docs/findings/279.

`splits.txt` is ours (`dol split --no-update` never rewrites it), so a gap is a real gap and
can be closed by adding an entry. Verified on fn_8_704 (title) and fn_12_23410
(movie_module), both of which stopped failing to link once their ranges existed.

Usage:
    python3 tools/fzgx/splitgaps.py [--module NAME] [--only-unmatched] [--json]
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from fzgx.project import ROOT  # noqa: E402

CONFIG = ROOT / "config" / "GFZE01"
RANGE_RE = re.compile(r"start:(0x[0-9A-Fa-f]+)\s+end:(0x[0-9A-Fa-f]+)")
FILE_RE = re.compile(r"^([\w/.\-]+\.c):\s*$")


def known_functions(module: str):
    """symbol -> (addr, size) from the module's symbols.txt."""
    out = {}
    path = CONFIG / module / "symbols.txt"
    if not path.exists():
        return out
    for line in path.read_text().splitlines():
        m = re.match(r"^(\w+) = \.text:(0x[0-9A-Fa-f]+);\s*//\s*type:function size:(0x[0-9A-Fa-f]+)",
                     line.strip())
        if m:
            out[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
    return out


def covered_ranges(module: str):
    """Set of (start, end) .text ranges the module's splits already cover."""
    path = CONFIG / module / "splits.txt"
    out = set()
    if not path.exists():
        return out
    for line in path.read_text().splitlines():
        if ".text" not in line:
            continue
        m = RANGE_RE.search(line)
        if m:
            out.add((int(m.group(1), 16), int(m.group(2), 16)))
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--module")
    ap.add_argument("--only-unmatched", action="store_true",
                    help="restrict to functions the ledger does not consider matched")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()

    status = {}
    if a.only_unmatched:
        from fzgx.ledger import Ledger
        status = {r["symbol"]: r["status"] for r in Ledger().db.execute(
            "SELECT symbol, status FROM functions")}

    modules = [a.module] if a.module else sorted(
        p.name for p in CONFIG.iterdir() if (p / "symbols.txt").exists())

    report = []
    for module in modules:
        cov = covered_ranges(module)
        for sym, (addr, size) in sorted(known_functions(module).items(), key=lambda kv: kv[1][0]):
            if a.only_unmatched and status.get(sym) == "matched":
                continue
            if size == 0 or any(s <= addr and addr + size <= e for s, e in cov):
                continue
            report.append(dict(module=module, symbol=sym, addr=addr, size=size,
                               end=addr + size, status=status.get(sym)))

    if a.json:
        print(json.dumps(report, indent=1))
        return 0
    if not report:
        print("no .text split gaps")
        return 0
    print("%-13s %-22s %-10s %8s" % ("module", "symbol", "start", "size"))
    for r in report:
        print("%-13s %-22s 0x%08X %8d" % (r["module"], r["symbol"], r["addr"], r["size"]))
    print("\n%d gap(s); add `rel/%s/%s.c:\\n\\t.text       start:0x%08X end:0x%08X align:4`"
          % (len(report), report[0]["module"], report[0]["symbol"],
             report[0]["addr"], report[0]["end"]))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())