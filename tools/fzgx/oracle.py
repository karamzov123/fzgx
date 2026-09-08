"""The oracle: compile one unit, diff it against retail, relink everything.

check(symbol)  -> compile the unit's object, objdiff it, return per-symbol
                  percentages and a compact instruction diff for `symbol`.
relink()       -> full `ninja`; the CHECK step fails unless every target in
                  build.sha1 is byte-identical.
"""

from __future__ import annotations

import fcntl
import json
import os
import subprocess
import sys
from contextlib import contextmanager
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional

from .project import ROOT, Project

OBJDIFF = ROOT / "build" / "tools" / "objdiff-cli"


@dataclass
class CheckResult:
    ok: bool  # compiled and objdiff ran
    symbol: str
    unit: str
    percent: float = 0.0
    matched: bool = False
    symbols: Dict[str, float] = field(default_factory=dict)  # every function in the unit
    data_sections: Dict[str, float] = field(default_factory=dict)
    diff: List[str] = field(default_factory=list)
    error: str = ""
    missing_in_base: List[str] = field(default_factory=list)
    extra_in_base: List[str] = field(default_factory=list)

    def to_json(self) -> dict:
        return self.__dict__


def run(cmd: List[str], cwd: Path = ROOT, timeout: int = 600) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, cwd=cwd, text=True, capture_output=True, timeout=timeout)


@contextmanager
def build_lock(name: str = "build.lock"):
    path = ROOT / "build" / "fzgx" / name
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w") as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        try:
            yield
        finally:
            fcntl.flock(f, fcntl.LOCK_UN)


def configure(project: Project) -> subprocess.CompletedProcess:
    return run([sys.executable, "configure.py", "--version", project.version])


def relink(project: Project) -> subprocess.CompletedProcess:
    """Full build; the CHECK step fails unless every target hashes identical."""
    return run(["ninja"], timeout=1800)


def _base_object(project: Project, unit: str) -> Path:
    units = project.objdiff_units()
    if unit in units and "base_path" in units[unit]:
        return ROOT / units[unit]["base_path"]
    module, rest = unit.split("/", 1)
    return project.build_dir / "src" / f"{rest}.o"


def _render_diff(left_rows: List[dict], right_rows: List[dict], max_lines: int) -> List[str]:
    """Side-by-side rendering: `target | ours`, with a marker on differing rows."""
    out: List[str] = []
    n = max(len(left_rows), len(right_rows))
    for i in range(n):
        l = left_rows[i] if i < len(left_rows) else {}
        r = right_rows[i] if i < len(right_rows) else {}
        kind = l.get("diff_kind") or r.get("diff_kind") or "DIFF_NONE"
        lf = l.get("instruction", {}).get("formatted", "")
        rf = r.get("instruction", {}).get("formatted", "")
        mark = " " if kind == "DIFF_NONE" else {"DIFF_REPLACE": "~", "DIFF_DELETE": "<", "DIFF_INSERT": ">",
                                                "DIFF_OP_MISMATCH": "!", "DIFF_ARG_MISMATCH": "?"}.get(kind, "*")
        if kind != "DIFF_NONE" or len(out) < 4:
            out.append(f"{mark} {i * 4:04X}  {lf:<34} | {rf}")
    if len(out) > max_lines:
        head = out[: max_lines - 1]
        head.append(f"... {len(out) - (max_lines - 1)} more differing rows")
        out = head
    return out


def check(project: Project, symbol: str, max_diff_lines: int = 80) -> CheckResult:
    sym = project.find_symbol(symbol)
    if sym is None:
        return CheckResult(False, symbol, "", error="unknown symbol")
    unit_src = project.unit_of(sym)
    if not unit_src:
        return CheckResult(False, symbol, "", error="function is not carved into a unit (fzgx carve)")
    unit = project.objdiff_unit_name(sym.module, unit_src)
    base_obj = _base_object(project, unit)

    with build_lock("compile.lock"):
        cp = run(["ninja", project.rel(base_obj)])
    if cp.returncode != 0:
        err = "\n".join(l for l in (cp.stdout + cp.stderr).splitlines()
                        if not l.startswith("[") and "Usage Warning" not in l)
        return CheckResult(False, symbol, unit, error=err.strip()[-4000:])

    cp = run([str(OBJDIFF), "diff", "-p", str(ROOT), "-u", unit, "-o", "-", "--format", "json"])
    if cp.returncode != 0:
        return CheckResult(False, symbol, unit, error=(cp.stderr or cp.stdout).strip()[-4000:])
    data = json.loads(cp.stdout)
    left, right = data.get("left", {}), data.get("right", {})
    res = CheckResult(True, symbol, unit)
    left_syms = {s["name"]: s for s in left.get("symbols", []) if s.get("kind") == "SYMBOL_FUNCTION"}
    right_syms = {s["name"]: s for s in right.get("symbols", []) if s.get("kind") == "SYMBOL_FUNCTION"}
    for name, s in left_syms.items():
        res.symbols[name] = float(s.get("match_percent", 0.0))
    res.missing_in_base = sorted(set(left_syms) - set(right_syms))
    res.extra_in_base = sorted(set(right_syms) - set(left_syms))
    for sec in left.get("sections", []):
        if sec.get("kind") in ("SECTION_DATA", "SECTION_BSS") and "match_percent" in sec:
            res.data_sections[sec["name"]] = float(sec["match_percent"])
    res.percent = res.symbols.get(symbol, 0.0)
    res.matched = res.percent >= 100.0
    if not res.matched:
        lrows = left_syms.get(symbol, {}).get("instructions", [])
        rrows = right_syms.get(symbol, {}).get("instructions", [])
        if symbol not in right_syms:
            res.diff = [f"(symbol {symbol} not present in our object: define it, check the name)"]
        else:
            res.diff = _render_diff(lrows, rrows, max_diff_lines)
    return res


def unit_fully_matches(res: CheckResult) -> Optional[str]:
    """Reason the unit may not be flipped to Matching, or None if it is safe."""
    if not res.ok:
        return res.error or "check failed"
    bad = [f"{n}={p:.1f}%" for n, p in res.symbols.items() if p < 100.0]
    if bad:
        return "functions below 100%: " + ", ".join(bad)
    if res.missing_in_base:
        return "missing from our object: " + ", ".join(res.missing_in_base)
    if res.extra_in_base:
        return "extra functions in our object: " + ", ".join(res.extra_in_base)
    bad_data = [f"{n}={p:.1f}%" for n, p in res.data_sections.items() if p < 100.0]
    if bad_data:
        return "data sections differ: " + ", ".join(bad_data)
    return None
