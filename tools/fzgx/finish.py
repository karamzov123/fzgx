"""The TU-finish pass: everything that turns matched blocks into whole translation units,
as one idempotent job over a module.

For every TU that has block units:
  1. its headers (globals.h and the TU header, when they exist) join the prologue;
  2. tidy: block-private declarations the headers cover are dropped;
  3. hoist: canonical extern declarations move to the prologue;
  4. reflag: every block's include set is re-decided (prologue, or self-contained);
  5. if every function of the TU is matched and the whole-file trial compiles with every
     block byte-identical, the TU is collapsed into one compiled unit (hash-verified).
Every step is verified per block by the object oracle; nothing is kept that stops matching.
Blocks that cannot compile under the prologue are the revise queue: the orchestrator
sends them to revise agents and runs the pass again. The pass ends with a relink; if the
hash fails, the TU files it touched are restored from git and the failure is reported.
"""

from __future__ import annotations

import json
import subprocess
from pathlib import Path
from typing import Dict, List

from . import collapse, oracle, tufile, tutidy, tutrial
from .project import ROOT, Project


def _tus(p: Project, module: str) -> List[str]:
    return sorted({u["tu"] for u in p.load_units() if u.get("tu") and u["module"] == module})


def _compile_ok(p: Project):
    return lambda u: oracle.compile_unit(p, p.objdiff_unit_name(u["module"], u["source"]), u["source"]).returncode == 0


def _all_matched(p: Project, tu_source: str) -> bool:
    module = tu_source.split("/")[1] if tu_source.startswith("rel/") else "main"
    t = collapse._tu_entry(p, module, tu_source)
    if t is None:
        return False
    st = {u["symbols"][0]: u["status"] for u in p.load_units() if u.get("tu") == tu_source}
    return all(st.get(f) == "matching" for f in t["functions"])


def finish_tu(p: Project, tu_source: str, do_collapse: bool = True) -> Dict[str, object]:
    module = tu_source.split("/")[1] if tu_source.startswith("rel/") else "main"
    stem = Path(tu_source).stem
    ok = _compile_ok(p)
    out: Dict[str, object] = {"tu": tu_source}
    for h in (f"rel/{module}/globals.h", f"rel/{module}/{stem}.h"):
        if (ROOT / "include" / h).exists():
            r = tufile.add_include(p, tu_source, h, ok)
            if r.get("added"):
                out.setdefault("included", []).append(h)
    t = tutidy.tidy(p, tu_source)
    out["tidied"] = len(t["tidied"])
    h = tutidy.hoist_decls(p, tu_source)
    out["hoisted"] = len(h["hoisted"])
    out["conflicts"] = len(h.get("conflicts", {}))
    r = tufile.reflag(p, tu_source, ok)
    out["flagged"], out["unflagged"] = len(r["flagged"]), len(r["unflagged"])
    tf = tufile.load(p, tu_source)
    out["queue"] = [b.name for b in tf.blocks if "noprologue" in b.flags]
    out["blocks"] = len(tf.blocks)
    if do_collapse and _all_matched(p, tu_source) and not out["queue"]:
        tr = tutrial.trial(p, tu_source)
        out["trial"] = {"ok": tr.get("ok"), "at_100": tr.get("at_100"), "functions": tr.get("functions")}
        if tr.get("ok") and tr.get("at_100") == tr.get("functions"):
            c = collapse.collapse(p, tu_source)
            out["collapsed"] = bool(c.get("ok"))
            if not c.get("ok"):
                out["collapse_error"] = c.get("error")
    return out


def finish(p: Project, module: str) -> Dict[str, object]:
    """The pass over every TU of the module, then one relink. Returns per-TU results, the
    revise queue and whether the tree still hashes."""
    tus = _tus(p, module)
    results = [finish_tu(p, tu) for tu in tus]
    queue = [s for r in results for s in r["queue"]]
    with oracle.build_lock():
        cp = oracle.configure(p)
        linked = cp.returncode == 0 and oracle.relink(p).returncode == 0
        if not linked:
            # nothing in this pass may leave the tree red: back to the committed TU files
            for tu in tus:
                cp = subprocess.run(["git", "show", f"HEAD:src/{tu}"], cwd=ROOT, text=True, capture_output=True)
                if cp.returncode == 0:
                    (ROOT / "src" / tu).write_text(cp.stdout)
            tufile.regenerate(p)
            oracle.configure(p)
            oracle.relink(p)
    collapsed = [r["tu"] for r in results if r.get("collapsed")]
    if linked:
        files = [str(ROOT / "src" / tu) for tu in tus] + [str(p.units_path), str(p.module_config_dir(module) / "splits.txt")]
        subprocess.run(["git", "add", *files], cwd=ROOT, capture_output=True)
        msg = f"tu-finish {module}: {sum(r['tidied'] for r in results)} tidied, {sum(r['hoisted'] for r in results)} hoisted, " \
              f"{len(queue)} queued for revise" + (f", collapsed {', '.join(Path(t).stem for t in collapsed)}" if collapsed else "")
        subprocess.run(["git", "commit", "-q", "-m", msg], cwd=ROOT, capture_output=True)
    return {"ok": linked, "module": module, "tus": len(tus), "queue": queue, "collapsed": collapsed,
            "tidied": sum(r["tidied"] for r in results), "hoisted": sum(r["hoisted"] for r in results),
            "flagged": sum(r["flagged"] for r in results), "unflagged": sum(r["unflagged"] for r in results),
            "results": results}
