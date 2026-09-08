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
import shlex
import struct
import subprocess
import sys
from contextlib import contextmanager
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional

from .project import ROOT, STATE_DIR, Project

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
    # every remaining diff is a relocation to a literal-pool constant whose value we reproduce
    # privately (an int->float conversion constant, or a literal shared with a neighbour):
    # per-function linking cannot express that; the TU's whole-unit compile will
    matched_pool: bool = False
    pool: List[str] = field(default_factory=list)

    def to_json(self) -> dict:
        return self.__dict__


def run(cmd: List[str], cwd: Path = ROOT, timeout: int = 600) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, cwd=cwd, text=True, capture_output=True, timeout=timeout)


@contextmanager
def build_lock(name: str = "build.lock"):
    path = STATE_DIR / name
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
    """Relink every target and verify build.sha1.

    Targets `build/<VERSION>/ok` (the dtk shasum check) rather than the default
    target, so only Matching units are compiled: another agent's broken
    in-progress unit cannot fail this step.
    """
    return run(["ninja", project.rel(project.build_dir / "ok")], timeout=1800)


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


def check(project: Project, symbol: str, max_diff_lines: int = 80, source: Optional[Path] = None) -> CheckResult:
    """Compile the unit (or `source`, an agent's work copy) into the unit's object and diff it."""
    sym = project.resolve(symbol)
    if sym is None:
        return CheckResult(False, symbol, "", error="unknown or ambiguous symbol (use module:name)")
    symbol = sym.name  # objdiff symbol name is the bare C name
    unit_src = project.unit_of(sym)
    if not unit_src:
        return CheckResult(False, symbol, "", error="function is not carved into a unit (fzgx carve)")
    unit = project.objdiff_unit_name(sym.module, unit_src)
    base_obj = _base_object(project, unit)

    # Direct mwcc compile into this unit's own object: no ninja, no build lock.
    cp = compile_unit(project, unit, unit_src, source)
    if cp.returncode != 0 or not base_obj.exists():
        err = "\n".join(l for l in (cp.stdout + cp.stderr).splitlines() if "Usage Warning" not in l)
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
            res.pool = _pool_only(project, sym.module, left, right, lrows, rrows)
            res.matched_pool = bool(res.pool)
    return res


def _pool_only(project: Project, module: str, left: dict, right: dict,
               lrows: List[dict], rrows: List[dict]) -> List[str]:
    """If every differing row is `same instruction, relocation to a pooled constant (retail)
    vs. to our private literal with the same bytes`, describe those constants; else []."""
    import base64
    if len(lrows) != len(rrows):
        return []
    lsyms, rsyms = left.get("symbols", []), right.get("symbols", [])
    syms = project.symbols(module)
    out: List[str] = []
    for l, r in zip(lrows, rrows):
        if (l.get("diff_kind") or "DIFF_NONE") == "DIFF_NONE" and (r.get("diff_kind") or "DIFF_NONE") == "DIFF_NONE":
            continue
        li, ri = l.get("instruction", {}), r.get("instruction", {})
        lrel, rrel = li.get("relocation"), ri.get("relocation")
        if not lrel or not rrel or lrel.get("type") != rrel.get("type"):
            return []
        # identical apart from the relocation target
        lp = [x for x in li.get("parts", []) if "reloc" not in json.dumps(x)]
        rp = [x for x in ri.get("parts", []) if "reloc" not in json.dumps(x)]
        if lp != rp:
            return []
        try:
            lname = lsyms[lrel["target_symbol"]]["name"]
            rsym = rsyms[rrel["target_symbol"]]
        except (IndexError, KeyError, TypeError):
            return []
        s = syms.get(lname)
        if not s or s.kind != "object" or s.section not in (".rodata", ".sdata2") or s.size not in (4, 8):
            return []
        retail = project.bytes_at(module, lname)
        ours = b"".join(base64.b64decode(d.get("data", "")) for d in rsym.get("data_diff", []))
        if not retail or ours != retail or not rsym.get("name", "").startswith("@"):
            return []
        v = struct.unpack(">d", retail)[0] if len(retail) == 8 else struct.unpack(">f", retail)[0]
        desc = f"{lname}={v!r}"
        if desc not in out:
            out.append(desc)
    return out


def unit_source_path(project: Project, unit_src: str) -> Path:
    """Where the unit's C lives: src/ for standalone units, build/<v>/gen/ for block units."""
    u = project.unit_record(unit_src)
    if u and u.get("tu"):
        return project.build_dir / "gen" / unit_src
    return ROOT / "src" / unit_src


def compile_unit(project: Project, unit: str, unit_src: str,
                 source: Optional[Path] = None) -> subprocess.CompletedProcess:
    """Compile one unit straight with mwcc (via wibo) into its objdiff base object.

    No ninja and no build lock: two agents compile two different files, so nothing
    is shared. The flags come from objdiff.json (the same ones ninja uses) plus the
    include dirs the ninja rule adds. ninja will still consider the object up to
    date at relink time because the object is newer than its source. `source`
    overrides the input (an agent's work copy, already assembled with the TU prologue).
    """
    meta = project.objdiff_units().get(unit, {})
    flags = meta.get("scratch", {}).get("c_flags", "").replace(" -lang=c", "")
    flags += f" -i include -i build/{project.version}/include"
    ucfg = project.unit_record(unit_src) or {}
    mw = ucfg.get("mw_version") or ("GC/1.2.5n" if unit.startswith("main/") else "GC/1.3.2")
    obj = _base_object(project, unit)
    obj.parent.mkdir(parents=True, exist_ok=True)
    src = source or unit_source_path(project, unit_src)
    cmd = [str(ROOT / "build" / "tools" / "wibo"), str(ROOT / "build" / "compilers" / mw / "mwcceppc.exe")]
    cmd += shlex.split(flags) + ucfg.get("extra_cflags", []) + ["-c", str(src), "-o", str(obj)]
    return subprocess.run(cmd, cwd=ROOT, text=True, capture_output=True, timeout=120)


CANDIDATE_VERSIONS = ["GC/1.2.5", "GC/1.2.5n", "GC/1.3", "GC/1.3.2", "GC/2.0", "GC/2.5", "GC/2.7"]


def check_versions(project: Project, symbol: str, versions: List[str]) -> Dict[str, float]:
    """Compile the unit under several compiler versions; return symbol match % per version.

    Uses the unit's flags from objdiff.json, wibo + build/compilers/<ver>/mwcceppc.exe,
    and objdiff-cli in two-object mode. Never touches the ninja build.
    """
    sym = project.resolve(symbol)
    unit_src = project.unit_of(sym) if sym else None
    if not sym or not unit_src:
        return {}
    symbol = sym.name
    unit = project.objdiff_unit_name(sym.module, unit_src)
    meta = project.objdiff_units().get(unit, {})
    # objdiff's scratch flags omit the include dirs the ninja rule adds per unit
    flags = meta.get("scratch", {}).get("c_flags", "").replace(" -lang=c", "")
    flags += f" -i include -i build/{project.version}/include"
    target = ROOT / meta.get("target_path", "")
    src = unit_source_path(project, unit_src)
    wibo = ROOT / "build" / "tools" / "wibo"
    out: Dict[str, float] = {}
    tmp = STATE_DIR / "versions" / symbol
    tmp.mkdir(parents=True, exist_ok=True)
    for ver in versions:
        mwcc = ROOT / "build" / "compilers" / ver / "mwcceppc.exe"
        if not mwcc.exists():
            out[ver] = -1.0
            continue
        obj = tmp / (ver.replace("/", "_") + ".o")
        # flags carry quoted pragmas: -pragma "cats off"
        cmd = [str(wibo), str(mwcc)] + shlex.split(flags) + ["-c", str(src), "-o", str(obj)]
        cp = subprocess.run(cmd, cwd=ROOT, text=True, capture_output=True)
        if cp.returncode != 0 or not obj.exists():
            (tmp / (ver.replace("/", "_") + ".err")).write_text(cp.stdout + cp.stderr)
            out[ver] = -2.0
            continue
        cp = run([str(OBJDIFF), "diff", "-1", str(target), "-2", str(obj), "-o", "-", "--format", "json"])
        if cp.returncode != 0:
            out[ver] = -3.0
            continue
        data = json.loads(cp.stdout)
        pct = 0.0
        for s in data.get("left", {}).get("symbols", []):
            if s.get("name") == symbol and "match_percent" in s:
                pct = float(s["match_percent"])
        out[ver] = pct
    return out


def unit_fully_matches(res: CheckResult) -> Optional[str]:
    """Reason the unit may not be flipped to Matching, or None if it is safe."""
    if not res.ok:
        return res.error or "check failed"
    bad = [f"{n}={p:.1f}%" for n, p in res.symbols.items() if p < 100.0 and not (res.matched_pool and n == res.symbol)]
    if bad:
        return "functions below 100%: " + ", ".join(bad)
    if res.missing_in_base:
        return "missing from our object: " + ", ".join(res.missing_in_base)
    if res.extra_in_base:
        return "extra functions in our object: " + ", ".join(res.extra_in_base)
    bad_data = [f"{n}={p:.1f}%" for n, p in res.data_sections.items()
                if p < 100.0 and not (res.matched_pool and n in (".rodata", ".sdata2"))]
    if bad_data:
        return "data sections differ: " + ", ".join(bad_data)
    return None
