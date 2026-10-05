"""Honest progress: what fraction of the project is actually written in C.

`fzgx report` reads objdiff's `matched_functions` / `complete_units`, and both are soft.
A function that was never decompiled still counts as matched when it sits inside a
**retail auto object**, whose bytes match by construction (docs/findings/280). So the
headline number includes code this project has never seen, and giving such a function its
own split *decreases* the counter while nothing regresses.

That makes the headline unusable for deciding whether a change helped. This module answers
the question the headline cannot: of the functions objdiff calls matched, how many have a C
body that is actually compiled into a target?

Four registries decide the answer, and they fail differently (findings 279/280):

    symbols.txt   retail address + size            (known function)
    splits.txt    which .text range a unit owns     (is there a unit at all?)
    units.json    that the unit is built from C     (is it compiled?)
    build/gen/    the generated translation unit   (was it actually built?)

A function is **authored** only when the ledger calls it matched, it has a units.json
record, and that unit's generated file exists. Only an authored function should count as
progress, and only an authored function can honestly be *completed*.
"""

from __future__ import annotations

import json
import re
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Set, Tuple

from .project import ROOT

CONFIG = ROOT / "config" / "GFZE01"
RANGE_RE = re.compile(r"start:(0x[0-9A-Fa-f]+)\s+end:(0x[0-9A-Fa-f]+)")
SYM_RE = re.compile(
    r"^(\w+) = \.text:(0x[0-9A-Fa-f]+);\s*//\s*type:function size:(0x[0-9A-Fa-f]+)")

_VERSION: Optional[str] = None
_SPLITS: Dict[str, Set[Tuple[int, int]]] = {}
_UNITS: Dict[str, dict] = {}
_SYMBOLS: Dict[str, Dict[str, Tuple[int, int]]] = {}


def version() -> str:
    """The configured GFZE01 build version directory name."""
    global _VERSION
    if _VERSION is None:
        from .project import Project
        _VERSION = Project().version
    return _VERSION


def reset() -> None:
    """Drop the caches. Call after editing splits.txt or units.json mid-process."""
    global _VERSION
    _VERSION = None
    _SPLITS.clear()
    _UNITS.clear()
    _SYMBOLS.clear()


def _modules() -> Iterable[str]:
    if not CONFIG.exists():
        return ()
    return sorted(p.name for p in CONFIG.iterdir() if (p / "symbols.txt").exists())


def symbols(module: str) -> Dict[str, Tuple[int, int]]:
    """module -> {symbol: (addr, size)} from symbols.txt."""
    if module not in _SYMBOLS:
        out: Dict[str, Tuple[int, int]] = {}
        path = CONFIG / module / "symbols.txt"
        if path.exists():
            for line in path.read_text().splitlines():
                m = SYM_RE.match(line.strip())
                if m:
                    out[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
        _SYMBOLS[module] = out
    return _SYMBOLS[module]


def covered(module: str) -> Set[Tuple[int, int]]:
    """module -> the .text ranges its splits.txt already covers."""
    if module not in _SPLITS:
        out: Set[Tuple[int, int]] = set()
        path = CONFIG / module / "splits.txt"
        if path.exists():
            for line in path.read_text().splitlines():
                if ".text" not in line:
                    continue
                m = RANGE_RE.search(line)
                if m:
                    out.add((int(m.group(1), 16), int(m.group(2), 16)))
        _SPLITS[module] = out
    return _SPLITS[module]


def units() -> Dict[str, dict]:
    """symbol -> its units.json record."""
    if not _UNITS:
        path = CONFIG / "units.json"
        if path.exists():
            for u in json.loads(path.read_text()):
                for sym in u.get("symbols") or ():
                    _UNITS.setdefault(sym, u)
    return _UNITS


def gen_built(record: Optional[dict]) -> bool:
    """True when this unit's object was actually built, i.e. the C reached the link.

    The object is the only artifact that proves the whole chain, and it is searched under
    every root any unit kind uses, because guessing one produced two large false negatives
    before this was measured: `gen/` alone reported all 645 movie_module functions
    unauthored, and `src/` alone then reported all 2,404 main_rel ones for the mirror-image
    reason. Verified roots for a `rel/<mod>/<unit>.c` record:

      build/<ver>/src/rel/<unit>.o    per-function units, every REL module
      build/<ver>/gen/rel/<unit>.o    block units, the only kind configure.py redirects
      build/<ver>/<mod>/obj/rel/...   main_rel's linked objects

    So probe all three and let the filesystem decide rather than encoding a guess.

    Requiring a built object is also what separates real work from a green build: a body in a
    TU file with no units.json entry leaves `configure.py` printing "Missing configuration for
    <unit>", skipping the unit, and the build still green while nothing of ours is linked
    (docs/findings/280).
    """
    if not record:
        return False
    src = record.get("source") or ""
    # `source` is `rel/<mod>/<unit>.c` and `<unit>` may contain slashes of its own
    # (`rel/main_rel/_prolog/fn_1_1280.c`), so the module comes from the record, not the path.
    rel = src[4:] if src.startswith("rel/") else src
    rel = rel[:-2] + ".o" if rel.endswith(".c") else rel
    module = record.get("module") or ""
    root = ROOT / "build" / version()
    candidates = [root / "src" / "rel" / rel, root / "gen" / "rel" / rel]
    if module:
        candidates.append(root / module / "obj" / "rel" / rel)
    return any(c.exists() for c in candidates)


def state(module: str, symbol: str) -> dict:
    """Per-function registry state, for one known symbol."""
    syms = symbols(module)
    if symbol not in syms:
        return {"module": module, "symbol": symbol, "known": False}
    addr, size = syms[symbol]
    rec = units().get(symbol)
    return {
        "module": module,
        "symbol": symbol,
        "known": True,
        "addr": addr,
        "size": size,
        "split": any(s <= addr and addr + size <= e for s, e in covered(module)),
        "registered": rec is not None,
        "compiled": gen_built(rec),
        "unit": rec.get("source") if rec else None,
    }


def authored(module: str, symbol: str) -> bool:
    """True when this symbol is built from a C body we wrote.

    Deliberately stricter than objdiff's `matched`: a split with no compiled unit leaves the
    retail auto object supplying the bytes, which matches by construction and is not work.
    """
    st = state(module, symbol)
    return bool(st.get("registered") and st.get("compiled"))


def _pct(a: float, b: float) -> float:
    return round(100.0 * a / b, 4) if b else 0.0


def _ledger_status() -> Dict[str, str]:
    from .ledger import Ledger
    return {r["symbol"]: r["status"] for r in Ledger().db.execute(
        "SELECT symbol, status FROM functions")}


def measure(status: Optional[Dict[str, str]] = None) -> dict:
    """Authored-vs-credited progress, overall and per module.

    `status` is symbol -> ledger status; when omitted the ledger is read. Only functions the
    ledger calls `matched` are candidates for credit, since an unmatched function has no
    accepted body to count even if a stale unit exists.
    """
    if status is None:
        status = _ledger_status()
    rec, rows = units(), []
    for module in _modules():
        for symbol, (addr, size) in symbols(module).items():
            if size == 0:
                continue
            r = rec.get(symbol)
            credited = status.get(symbol) == "matched"
            rows.append({
                "module": module, "symbol": symbol, "size": size,
                "credited": credited,
                "authored": bool(credited and r is not None and gen_built(r)),
                "registered": r is not None,
                "compiled": gen_built(r) if r else False,
                "split": any(s <= addr and addr + size <= e for s, e in covered(module)),
            })
    return _summarise(rows, status)


def _summarise(rows: List[dict], status: Dict[str, str]) -> dict:
    keys = ("known", "credited", "authored",
            "known_bytes", "credited_bytes", "authored_bytes")
    per: Dict[str, dict] = {}
    for r in rows:
        m = per.setdefault(r["module"], {k: 0 for k in keys})
        m["known"] += 1
        m["known_bytes"] += r["size"]
        if r["credited"]:
            m["credited"] += 1
            m["credited_bytes"] += r["size"]
        if r["authored"]:
            m["authored"] += 1
            m["authored_bytes"] += r["size"]
    tot = {k: sum(m[k] for m in per.values()) for k in keys}
    return {
        "total": tot,
        "per_module": per,
        "unmatched_ledger": sum(1 for s in status.values() if s != "matched"),
        # Of the functions objdiff credits, the share we can point at real source for.
        "authored_of_credited_percent": _pct(tot["authored"], tot["credited"]),
        "authored_code_percent": _pct(tot["authored_bytes"], tot["known_bytes"]),
        "credited_code_percent": _pct(tot["credited_bytes"], tot["known_bytes"]),
        "functions": rows,
    }


def uncredited() -> List[dict]:
    """Functions objdiff credits that this project never wrote: the soft-number gap.

    Each row is a candidate for real work, and every one is a place the headline number is
    currently lying by exactly one function.
    """
    return [r for r in measure()["functions"] if r["credited"] and not r["authored"]]