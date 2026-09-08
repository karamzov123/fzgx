"""decomp-permuter on a plateaued attempt.

A function that a cheap model leaves at 90–99% usually differs from retail by
register allocation or statement order, which is what random source
permutation explores well and a language model badly. This module turns the
best saved attempt of a function into a permuter directory, runs the permuter
for a bounded time, and if a permutation scores 0 (byte-identical), writes it
back as the function's work copy and submits it through the normal oracle.

Inputs the permuter needs per directory: `base.c` (one function, self-contained
and preprocessed, so MWCC's own `-E` output), `target.o` (dtk's retail split
object for the unit), `compile.sh`, `settings.toml`. It scores with a GNU
objdump for powerpc-eabi (build/tools/binutils, built by `fzgx permute --setup`).
"""

from __future__ import annotations

import os
import re
import shlex
import shutil
import subprocess
import sys
import time
from pathlib import Path
from typing import Dict, List, Optional

from . import api, oracle, tufile
from .ledger import Ledger
from .project import ROOT, STATE_DIR, Project

PERMUTER = ROOT / "build" / "tools" / "decomp-permuter" / "permuter.py"
OBJDUMP = ROOT / "build" / "tools" / "binutils" / "bin" / "powerpc-eabi-objdump"
LINE_COMMENT_RE = re.compile(r"^/\* #line .*$", re.M)


def _attempt_text(p: Project, key: str) -> Optional[str]:
    """The best saved body: the live work copy, else .best.c, else the ledger's last attempt."""
    work = p.work_path(key)
    if work.exists():
        return work.read_text()
    best = STATE_DIR / "attempts" / f"{key}.best.c"
    if best.exists():
        return best.read_text()
    row = Ledger().db.execute(
        "SELECT best_body_path FROM attempts WHERE symbol=? AND best_body_path IS NOT NULL "
        "ORDER BY final_percent DESC, id DESC LIMIT 1", (key,)).fetchone()
    if row and row[0]:
        path = Path(row[0])
        if not path.exists():  # the repository moved; the store did not
            path = STATE_DIR / "attempts" / path.name
        if path.exists():
            return path.read_text()
    return None


def _flags(p: Project, module: str, unit_src: str) -> tuple[str, str]:
    unit = p.objdiff_unit_name(module, unit_src)
    meta = p.objdiff_units().get(unit, {})
    flags = meta.get("scratch", {}).get("c_flags", "").replace(" -lang=c", "")
    flags += f" -i include -i build/{p.version}/include"
    ucfg = p.unit_record(unit_src) or {}
    mw = ucfg.get("mw_version") or ("GC/1.2.5n" if module == "main" else "GC/1.3.2")
    return flags + " " + " ".join(ucfg.get("extra_cflags", [])), mw


def _assemble(p: Project, unit_src: str, text: str) -> str:
    u = p.unit_record(unit_src)
    if u and u.get("tu"):
        tf = tufile.load(p, u["tu"])
        inc, body = tufile.split_includes(text)
        return tufile.merge_prologue(tf.prologue, inc) + "\n" + body
    return text


def _preprocess(p: Project, mw: str, flags: str, src: Path, out: Path) -> str:
    cmd = [str(ROOT / "build/tools/wibo"), str(ROOT / "build/compilers" / mw / "mwcceppc.exe")]
    cmd += shlex.split(flags) + ["-E", "-o", str(out), str(src)]
    cp = subprocess.run(cmd, cwd=ROOT, text=True, capture_output=True, timeout=120)
    if cp.returncode != 0 or not out.exists():
        raise RuntimeError("preprocess failed: " + (cp.stdout + cp.stderr)[-1500:])
    text = LINE_COMMENT_RE.sub("", out.read_text())
    text = re.sub(r"\n{3,}", "\n\n", text)
    out.write_text(text)
    return text


def prepare(p: Project, symbol: str) -> Dict[str, object]:
    sym = p.resolve(symbol)
    if sym is None:
        return {"ok": False, "error": "unknown symbol"}
    key = p.key(sym)
    unit_src = p.unit_of(sym)
    if not unit_src:
        r = api.carve_many(p, [symbol])
        unit_src = p.unit_of(sym)
        if not unit_src:
            return {"ok": False, "error": f"carve failed: {r}"}
    text = _attempt_text(p, key)
    if not text or sym.name not in text:
        return {"ok": False, "error": "no saved attempt that defines the function"}
    flags, mw = _flags(p, sym.module, unit_src)
    unit = p.objdiff_unit_name(sym.module, unit_src)
    target = ROOT / p.objdiff_units().get(unit, {}).get("target_path", "")
    if not target.exists():
        return {"ok": False, "error": f"no retail object for {unit} (run ninja)"}
    d = STATE_DIR / "permute" / key.replace(":", "__")
    if d.exists():
        shutil.rmtree(d)
    d.mkdir(parents=True)
    # an attempt is a complete unit (its own includes); the TU prologue is only a fallback
    raw = d / "attempt.c"
    raw.write_text(text)
    _write_compile(d, mw, flags)
    try:
        _preprocess(p, mw, flags, raw, d / "base.c")
    except RuntimeError as e:
        return {"ok": False, "error": str(e)}
    cp = subprocess.run(["sh", str(d / "compile.sh"), str(d / "base.c"), "", str(d / "base.o")],
                        cwd=ROOT, text=True, capture_output=True, timeout=120)
    if cp.returncode != 0:
        raw.write_text(_assemble(p, unit_src, text))
        try:
            _preprocess(p, mw, flags, raw, d / "base.c")
        except RuntimeError as e:
            return {"ok": False, "error": str(e)}
        cp = subprocess.run(["sh", str(d / "compile.sh"), str(d / "base.c"), "", str(d / "base.o")],
                            cwd=ROOT, text=True, capture_output=True, timeout=120)
        if cp.returncode != 0:
            err = "\n".join(l for l in (cp.stdout + cp.stderr).splitlines() if "Usage Warning" not in l)
            return {"ok": False, "error": "attempt does not compile: " + err[-1200:]}
    shutil.copy(target, d / "target.o")
    (d / "settings.toml").write_text(f'func_name = "{sym.name}"\ncompiler_type = "mwcc"\n')
    return {"ok": True, "dir": p.rel(d), "unit": unit_src, "key": key}


def _write_compile(d: Path, mw: str, flags: str) -> None:
    (d / "compile.sh").write_text(
        "#!/bin/sh\n"
        f"cd {ROOT}\n"
        f"exec build/tools/wibo build/compilers/{mw}/mwcceppc.exe {flags} -c -o \"$3\" \"$1\"\n")
    (d / "compile.sh").chmod(0o755)


def _function_span(text: str, name: str) -> Optional[tuple[int, int]]:
    """[start, end) of the definition of `name` in `text` (brace matched)."""
    m = re.search(rf"^[^\n;{{}}]*\b{re.escape(name)}\s*\([^;{{)]*\)\s*\{{", text, re.M)
    if not m:
        return None
    i, depth = m.end() - 1, 0
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return m.start(), i + 1
        i += 1
    return None


def relocation_only(diff: List[str]) -> bool:
    """True when every differing row is a symbol/literal mismatch: retail references a pooled
    constant or another symbol where ours emits a private literal (or vice versa). Permutation
    cannot change that, and the permuter's objdump scoring does not even see it."""
    rows = [r for r in diff if r[:1] in "?~<>!*"]
    if not rows:
        return False
    return all(("@" in r and ("lbl_" in r or "@" in r.split("|")[1])) and "@ha" in r or "@l" in r for r in rows)


def run(p: Project, symbol: str, threads: int = 8, seconds: int = 600, submit: bool = True) -> Dict[str, object]:
    prep = prepare(p, symbol)
    if not prep["ok"]:
        return prep
    d = ROOT / prep["dir"]
    # objdiff first: a relocation-only diff is a hint for the model, not a permutation
    key = prep["key"]
    work = p.work_path(key)
    had_work = work.exists()
    if not had_work:
        work.parent.mkdir(parents=True, exist_ok=True)
        work.write_text((d / "attempt.c").read_text())
    pre = oracle.check(p, symbol, 200, source=work)
    if not had_work:
        work.unlink(missing_ok=True)
    if pre.ok and relocation_only(pre.diff):
        return {"ok": True, "symbol": symbol, "secs": 0.0, "skipped": "relocation-only diff (constant pool or symbol reference); not permutable",
                "percent": pre.percent, "diff": pre.diff[:6]}
    t0 = time.time()
    cmd = [sys.executable, str(PERMUTER), str(d), "-j", str(threads), "--stop-on-zero", "--best-only", "--quiet"]
    env = dict(os.environ, PATH=f"{OBJDUMP.parent}:{os.environ.get('PATH', '')}")  # powerpc-eabi-objdump
    try:
        cp = subprocess.run(cmd, cwd=ROOT, text=True, capture_output=True, timeout=seconds, env=env)
        out = cp.stdout + cp.stderr
    except subprocess.TimeoutExpired as e:
        out = (e.stdout or b"").decode(errors="replace") if isinstance(e.stdout, bytes) else (e.stdout or "")
        out += (e.stderr or b"").decode(errors="replace") if isinstance(e.stderr, bytes) else (e.stderr or "")
    elapsed = round(time.time() - t0, 1)
    scores = [int(s) for s in re.findall(r"found new best score:? (\d+)", out)]
    base = re.search(r"base score:? (\d+)", out)
    zero = sorted(d.glob("output-0-*/source.c"))
    result: Dict[str, object] = {"ok": True, "symbol": symbol, "secs": elapsed,
                                 "base_score": int(base.group(1)) if base else None,
                                 "best_score": min(scores) if scores else None,
                                 "iterations": (re.findall(r"iteration (\d+)", out) or ["?"])[-1],
                                 "tail": out.strip().splitlines()[-3:]}
    if not zero:
        return result
    # graft the permuted function back onto the readable attempt (the output is preprocessed C)
    sym = p.resolve(symbol)
    attempt = (d / "attempt.c").read_text()
    permuted = zero[0].read_text()
    a, b = _function_span(attempt, sym.name), _function_span(permuted, sym.name)
    if not a or not b:
        result["error"] = "could not locate the function in the attempt or the output"
        return result
    text = attempt[:a[0]] + permuted[b[0]:b[1]] + attempt[a[1]:]
    key = prep["key"]
    work = p.work_path(key)
    work.parent.mkdir(parents=True, exist_ok=True)
    work.write_text(text if text.endswith("\n") else text + "\n")
    chk = api.check(p, symbol)
    result["check"] = chk.get("percent")
    if chk.get("ok") and chk.get("matched") and submit:
        result["submit"] = api.submit(p, symbol, agent="permuter", message="decomp-permuter",
                                      harness="permuter", model="decomp-permuter")
    return result


def plateau(p: Project, module: Optional[str] = None, min_percent: float = 90.0, max_size: int = 1024,
            limit: int = 20) -> List[Dict[str, object]]:
    l = Ledger()
    q = ("SELECT symbol FROM functions WHERE status='unmatched' AND best_percent>=? AND size<=? "
         + ("AND module=? " if module else "") + "ORDER BY best_percent DESC, size LIMIT ?")
    args = [min_percent, max_size] + ([module] if module else []) + [limit]
    return [{"symbol": r[0]} for r in l.db.execute(q, args)]
