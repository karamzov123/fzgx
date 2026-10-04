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

**A split range is necessary but not sufficient** (docs/findings/280). A second registry,
`config/GFZE01/units.json`, decides whether the range is built from C at all, and nothing in
the check path consults it. The three registries fail differently:

  no split range          `check` reports 100%, submit fails at link (finding 279)
  units.json but no body  link fails: `Failed to find symbol <sym> in any module`
  body but no units.json  `configure.py` prints `Missing configuration for <unit>`, skips
                          the unit, `gen/<unit>.c` is never written, and the build stays
                          green while the body is absent from the link

`--registries` reports all three per function, so an object-100% body can be triaged before
another attempt is spent on it. A green build is not evidence about code that was never
compiled, so treat a missing registry as the explanation until proven otherwise.

**Not every gap should be closed.** Giving a function its own split makes it its own link
unit; if the functions it calls are still in auto units the linker places after it, the
module fails to resolve with a cyclic-dependency error. `config/GFZE01/cyclic_splits.json`
holds the functions measured to do this, each confirmed by rebuilding, and they are
reported as `CYCLIC` rather than offered as an addable gap. They need a call-graph-aware
split. See docs/findings/280.

Usage:
    python3 tools/fzgx/splitgaps.py [--module NAME] [--only-unmatched] [--registries] [--json]
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Optional

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from fzgx.project import ROOT  # noqa: E402

CONFIG = ROOT / "config" / "GFZE01"
RANGE_RE = re.compile(r"start:(0x[0-9A-Fa-f]+)\s+end:(0x[0-9A-Fa-f]+)")
FILE_RE = re.compile(r"^([\w/.\-]+\.c):\s*$")
CYCLIC_PATH = CONFIG / "cyclic_splits.json"
_CYCLIC: Optional[set] = None


def cyclic_symbols() -> set:
    """Functions measured to create a link-order cycle when given their own split.

    A new split makes a function its own link unit. If the functions it calls are still
    supplied by auto units the linker must place after it, resolution is circular and the
    module does not link (docs/findings/280). These were each confirmed by rebuilding, so
    do not propose a split for them: they need a call-graph-aware split, not a range.
    """
    global _CYCLIC
    if _CYCLIC is None:
        try:
            _CYCLIC = set(json.loads(CYCLIC_PATH.read_text()).get("symbols") or ())
        except (OSError, ValueError):
            _CYCLIC = set()
    return _CYCLIC


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


_UNITS_CACHE: Optional[dict] = None


def _units_index():
    """symbol -> units.json record, for the module-registry half of the diagnosis.

    Cached: the index is consulted once per known function, and re-parsing the 5.7k-entry
    file each time turns a full run into minutes.
    """
    global _UNITS_CACHE
    if _UNITS_CACHE is not None:
        return _UNITS_CACHE
    path = CONFIG / "units.json"
    out: dict = {}
    if path.exists():
        for u in json.loads(path.read_text()):
            for sym in u.get("symbols") or ():
                out.setdefault(sym, u)
    _UNITS_CACHE = out
    return out


def _gen_built(record: Optional[dict]) -> bool:
    """True when `build/<V>/gen/<unit>.c` exists, i.e. the unit really is compiled.

    A body in the TU file with no units.json entry leaves this missing, and the build stays
    green while nothing of ours reaches the link (docs/findings/280). The unit's `source`
    is the path *relative to rel/*, and `gen/` mirrors it, so the check follows `source`
    rather than guessing from the symbol name.
    """
    if not record:
        return False
    src = record.get("source") or ""
    rel = src.split("/", 1)[1] if src.startswith("rel/") else src
    return (ROOT / "build" / "GFZE01" / "gen" / "rel" / rel).exists()


def registry_state(module: str, sym: str, addr: int, size: int, cov) -> dict:
    """The three registries, so a link failure can be attributed before another attempt."""
    rec = _units_index().get(sym)
    has_split = any(s <= addr and addr + size <= e for s, e in cov)
    return {
        "module": module,
        "symbol": sym,
        "addr": addr,
        "size": size,
        "end": addr + size,
        "split": has_split,
        "unit": rec.get("source") if rec else None,
        "registered": rec is not None,
        "compiled": _gen_built(rec),
    }


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--module")
    ap.add_argument("--only-unmatched", action="store_true",
                    help="restrict to functions the ledger does not consider matched")
    ap.add_argument("--registries", action="store_true",
                    help="report split/units.json/gen state per function, not just split gaps")
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
            if size == 0:
                continue
            if a.registries:
                report.append(registry_state(module, sym, addr, size, cov))
            elif any(s <= addr and addr + size <= e for s, e in cov):
                continue
            elif sym in cyclic_symbols():
                # measured to fail the link; a range is not the fix
                report.append(dict(module=module, symbol=sym, addr=addr, size=size,
                                   end=addr + size, status=status.get(sym), cyclic=True))
            else:
                report.append(dict(module=module, symbol=sym, addr=addr, size=size,
                                   end=addr + size, status=status.get(sym)))

    if a.json:
        print(json.dumps(report, indent=1))
        return 0
    if not report:
        print("no .text split gaps" if not a.registries else "no functions to report")
        return 0
    if a.registries:
        print("%-13s %-22s %6s %-28s %6s %s"
              % ("module", "symbol", "split", "unit", "gen", "verdict"))
        for r in report:
            verdict = ""
            if not r["split"] and not r["registered"]:
                verdict = "no split, no units.json entry"
            elif not r["split"] and r["registered"] and not r["compiled"]:
                verdict = "registered, no gen unit"
            elif not r["split"] and r["registered"]:
                verdict = "registered but no split range"
            elif r["split"] and not r["registered"]:
                verdict = "split only (object comes from retail)"
            elif r["registered"] and not r["compiled"]:
                verdict = "body present, not regenerated"
            if r["symbol"] in cyclic_symbols():
                verdict = (verdict + "; " if verdict else "") + "CYCLIC: a split here fails the link"
            print("%-13s %-22s %6s %-28s %6s %s"
                  % (r["module"], r["symbol"], "yes" if r["split"] else "NO",
                     (r["unit"] or "-")[:28], "yes" if r["compiled"] else "NO", verdict))
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