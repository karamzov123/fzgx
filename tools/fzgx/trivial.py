"""Mechanical matches that need no model: single-`blr` bodies and `li r3, N; blr`.

Carves every candidate, writes the C, flips all of them to Matching, relinks
once, verifies every hash, records them in the ledger and commits once. If
the bulk relink fails, falls back to one-at-a-time submits so the culprit is
isolated and the rest still land.
"""

from __future__ import annotations

import re
import subprocess
from typing import Dict, List, Optional, Tuple

from . import api, oracle
from .carve import carve
from .ledger import Ledger
from .project import ROOT, Project

LI_RE = re.compile(r"^li r3, (-?0x[0-9a-fA-F]+|-?\d+)$")


def body_for(asm: List[str], name: str) -> Optional[str]:
    insns = [l.split(": ", 1)[1] if ": " in l else l for l in asm if not l.endswith(":")]
    if insns == ["blr"]:
        return f"// {name}: empty in retail (single blr).\nvoid {name}(void) {{\n}}\n"
    if len(insns) == 2 and insns[1] == "blr":
        m = LI_RE.match(insns[0])
        if m:
            v = int(m.group(1), 0)
            return f"// {name}: returns a constant.\nint {name}(void) {{\n    return {v};\n}}\n"
    return None


def candidates(p: Project, modules: Optional[List[str]] = None) -> List[Tuple[str, str, str]]:
    """(module, symbol, body) for every unmatched function with a trivial body."""
    l = Ledger()
    out = []
    for module in modules or p.modules:
        index = p.function_asm(module)
        for sym in p.functions(module):
            row = l.get(p.key(sym))
            if row is None or row["status"] != "unmatched":
                continue
            fn = index.get(sym.name)
            if fn is None:
                continue
            body = body_for(fn.asm, sym.name)
            if body:
                out.append((module, p.key(sym), body))
    return out


def apply(p: Project, modules: Optional[List[str]] = None, limit: Optional[int] = None,
          dry_run: bool = False) -> Dict[str, object]:
    cands = candidates(p, modules)
    if limit:
        cands = cands[:limit]
    if dry_run or not cands:
        return {"candidates": len(cands), "symbols": [c[1] for c in cands], "applied": 0}

    l = Ledger()
    sources: Dict[str, str] = {}
    with oracle.build_lock():
        for module, symbol, body in cands:
            l.claim(symbol, "trivial", ttl=3600, max_attempts=99)
            r = carve(p, symbol)
            (ROOT / "src" / r.source).write_text('#include "types.h"\n\n' + body)
            sources[symbol] = r.source
        units = p.load_units()
        for u in units:
            if u["source"] in sources.values():
                u["status"] = "matching"
        p.save_units(units)
        cp = oracle.configure(p)
        if cp.returncode == 0:
            cp = oracle.relink(p)
        bulk_ok = cp.returncode == 0
        if not bulk_ok:
            for u in units:
                if u["source"] in sources.values():
                    u["status"] = "nonmatching"
            p.save_units(units)
            oracle.configure(p)

    matched: List[str] = []
    failed: List[Tuple[str, str]] = []
    if bulk_ok:
        files = [str(ROOT / "src" / s) for s in sources.values()]
        cfg = [str(p.units_path), str(p.config_dir / "config.yml")]
        for module in {m for m, _, _ in cands}:
            d = p.module_config_dir(module)
            cfg += [str(d / "splits.txt"), str(d / "symbols.txt")]
        subprocess.run(["git", "add", *files, *cfg], cwd=ROOT, capture_output=True)
        msg = f"match: {len(cands)} trivial functions (blr / li r3,N) across {len({m for m, _, _ in cands})} modules"
        subprocess.run(["git", "commit", "-q", "-m", msg], cwd=ROOT, capture_output=True)
        commit = subprocess.run(["git", "rev-parse", "--short", "HEAD"], cwd=ROOT, text=True,
                                capture_output=True).stdout.strip()
        for _, symbol, _ in cands:
            l.finish(symbol, "matched", "matched", commit=commit, notes="trivial", model="none", harness="fzgx")
            matched.append(symbol)
    else:
        for _, symbol, _ in cands:
            r = api.submit(p, symbol, agent="trivial", message="trivial", harness="fzgx", model="none")
            if r.get("ok"):
                matched.append(symbol)
            else:
                failed.append((symbol, str(r.get("error"))))
                l.finish(symbol, "released", "unmatched", notes=str(r.get("error"))[:200])
    return {"candidates": len(cands), "applied": len(matched), "failed": failed, "bulk_ok": bulk_ok}
