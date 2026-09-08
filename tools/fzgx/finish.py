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

import hashlib
import json
import re
import subprocess
import threading
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
from typing import Dict, List, Optional

from . import collapse, oracle, tufile, tutidy, tutrial
from .project import ROOT, STATE_DIR, Project


def _tus(p: Project, module: str) -> List[str]:
    return sorted({u["tu"] for u in p.load_units() if u.get("tu") and u["module"] == module})


class Verifier:
    """Compile and match verdicts memoised by the generated unit's text: an unchanged block
    costs nothing on the next pass. Persisted under .fzgx/verify_cache.json."""

    def __init__(self, p: Project):
        self.p = p
        self.path = STATE_DIR / "verify_cache.json"
        self.lock = threading.Lock()
        try:
            self.cache: Dict[str, dict] = json.loads(self.path.read_text()) if self.path.exists() else {}
        except ValueError:
            self.cache = {}
        self.hits = self.misses = 0

    def _key(self, u: dict, kind: str) -> str:
        gen = tufile.gen_path(self.p, u["source"])
        text = gen.read_text() if gen.exists() else ""
        h = hashlib.sha256((u["source"] + "\0" + (u.get("mw_version") or "") + "\0" + " ".join(u.get("extra_cflags") or [])
                            + "\0" + json.dumps(u.get("pool") or {}, sort_keys=True) + "\0" + text).encode()).hexdigest()
        return f"{kind}:{h}"

    def compiles(self, u: dict) -> bool:
        k = self._key(u, "compile")
        with self.lock:
            if k in self.cache:
                self.hits += 1
                return self.cache[k]["ok"]
        self.misses += 1
        ok = oracle.compile_unit(self.p, self.p.objdiff_unit_name(u["module"], u["source"]), u["source"]).returncode == 0
        with self.lock:
            self.cache[k] = {"ok": ok}
        return ok

    def matches(self, p: Project, name: str) -> bool:
        u = next((x for x in p.load_units() if x["symbols"] and x["symbols"][0] == name), None)
        if u is None:
            return False
        k = self._key(u, "match")
        with self.lock:
            if k in self.cache:
                self.hits += 1
                return self.cache[k]["ok"]
        self.misses += 1
        res = oracle.check(p, name, 20)
        ok = res.ok and (res.matched or res.matched_pool) and oracle.unit_fully_matches(res) is None
        with self.lock:
            self.cache[k] = {"ok": ok, "pool_map": res.pool_map if res.ok else {}}
        return ok

    def save(self) -> None:
        with self.lock:
            self.path.parent.mkdir(parents=True, exist_ok=True)
            self.path.write_text(json.dumps(self.cache))


def _all_matched(p: Project, tu_source: str) -> bool:
    module = tu_source.split("/")[1] if tu_source.startswith("rel/") else "main"
    t = collapse._tu_entry(p, module, tu_source)
    if t is None:
        return False
    st = {u["symbols"][0]: u["status"] for u in p.load_units() if u.get("tu") == tu_source}
    return all(st.get(f) == "matching" for f in t["functions"])


def finish_tu(p: Project, tu_source: str, do_collapse: bool = True, v: Optional[Verifier] = None) -> Dict[str, object]:
    module = tu_source.split("/")[1] if tu_source.startswith("rel/") else "main"
    stem = Path(tu_source).stem
    v = v or Verifier(p)
    ok = v.compiles
    out: Dict[str, object] = {"tu": tu_source}
    for h in (f"rel/{module}/globals.h", f"rel/{module}/{stem}.h"):
        if (ROOT / "include" / h).exists():
            r = tufile.add_include(p, tu_source, h, ok)
            if r.get("added"):
                out.setdefault("included", []).append(h)
    t = tutidy.tidy(p, tu_source, check_fn=v.matches)
    out["tidied"] = len(t["tidied"])
    h = tutidy.hoist_decls(p, tu_source, check_fn=v.matches)
    out["hoisted"] = len(h["hoisted"])
    out["conflicts"] = len(h.get("conflicts", {}))
    r = tufile.reflag(p, tu_source, ok)
    out["flagged"], out["unflagged"] = len(r["flagged"]), len(r["unflagged"])
    tf = tufile.load(p, tu_source)
    out["queue"] = [b.name for b in tf.blocks if "noprologue" in b.flags]
    out["blocks"] = len(tf.blocks)
    # a tidied block may renumber its private literals: keep the pool mappings current
    for u in p.load_units():
        if u.get("tu") == tu_source and isinstance(u.get("pool"), dict):
            res = oracle.check(p, u["symbols"][0], 20)
            if res.ok and res.matched and res.pool_map and res.pool_map != u["pool"]:
                with oracle.build_lock("units.lock"):
                    units = p.load_units()
                    for x in units:
                        if x["source"] == u["source"]:
                            x["pool"] = dict(res.pool_map)
                    p.save_units(units)
                out.setdefault("pool_remapped", []).append(u["symbols"][0])
    if do_collapse and _all_matched(p, tu_source) and not out["queue"]:
        tr = tutrial.trial(p, tu_source)
        out["trial"] = {"ok": tr.get("ok"), "at_100": tr.get("at_100"), "functions": tr.get("functions")}
        if tr.get("ok") and tr.get("at_100") == tr.get("functions"):
            c = collapse.collapse(p, tu_source)
            out["collapsed"] = bool(c.get("ok"))
            if not c.get("ok"):
                out["collapse_error"] = c.get("error")
    return out


def finish(p: Project, module: str, workers: int = 12) -> Dict[str, object]:
    """The pass over every TU of the module, then one relink. Returns per-TU results, the
    revise queue and whether the tree still hashes."""
    tus = _tus(p, module)
    v = Verifier(p)
    t0 = time.time()
    # TUs are independent files with their own locks; only collapse takes the build lock
    with ThreadPoolExecutor(max_workers=workers) as ex:
        results = list(ex.map(lambda tu: finish_tu(p, tu, True, v), tus))
    v.save()
    queue = [s for r in results for s in r["queue"]]
    diag: Dict[str, object] = {}
    restored: List[str] = []
    with oracle.build_lock():
        cp = oracle.configure(p)
        cp2 = oracle.relink(p, keep_going=True) if cp.returncode == 0 else cp
        linked = cp.returncode == 0 and cp2.returncode == 0
        if not linked:
            # name the culprits, restore only their TU files, keep the rest of the pass
            out = cp2.stdout + cp2.stderr
            failed = re.findall(r"FAILED: \[code=\d+\] build/\S+/src/(\S+?)\.o", out)
            diag = {"failed_units": failed[:20], "errors": [l for l in out.splitlines() if l.startswith("#   ") and "Error" not in l][:12]}
            tmap = p.tu_map(module)
            units_by_src = {u["source"]: u for u in p.load_units()}
            bad_tus = sorted({units_by_src[f + ".c"]["tu"] for f in failed if f + ".c" in units_by_src and units_by_src[f + ".c"].get("tu")})
            if not failed:  # the link ran: a hash mismatch, name it by function
                diag = oracle.byte_diff(p, module)
                bad_tus = sorted({f"{p.module_src_prefix(module)}/{tmap[f]}.c" for f, _ in diag.get("text_diffs", []) if f in tmap})
                if not bad_tus or diag.get("sections"):
                    bad_tus = list(tus)  # a layout shift: no single function to blame
            for tu in bad_tus:
                cp = subprocess.run(["git", "show", f"HEAD:src/{tu}"], cwd=ROOT, text=True, capture_output=True)
                if cp.returncode == 0:
                    (ROOT / "src" / tu).write_text(cp.stdout)
                    restored.append(tu)
            tufile.regenerate(p)
            oracle.configure(p)
            linked = oracle.relink(p).returncode == 0
            if not linked:  # still red: everything back
                for tu in tus:
                    cp = subprocess.run(["git", "show", f"HEAD:src/{tu}"], cwd=ROOT, text=True, capture_output=True)
                    if cp.returncode == 0:
                        (ROOT / "src" / tu).write_text(cp.stdout)
                restored = list(tus)
                tufile.regenerate(p)
                oracle.configure(p)
                linked = oracle.relink(p).returncode == 0
    collapsed = [r["tu"] for r in results if r.get("collapsed")]
    if linked:
        files = [str(ROOT / "src" / tu) for tu in tus] + [str(p.units_path), str(p.module_config_dir(module) / "splits.txt")]
        subprocess.run(["git", "add", *files], cwd=ROOT, capture_output=True)
        msg = f"tu-finish {module}: {sum(r['tidied'] for r in results)} tidied, {sum(r['hoisted'] for r in results)} hoisted, " \
              f"{len(queue)} queued for revise" + (f", collapsed {', '.join(Path(t).stem for t in collapsed)}" if collapsed else "")
        subprocess.run(["git", "commit", "-q", "-m", msg], cwd=ROOT, capture_output=True)
    return {"ok": linked, "module": module, "tus": len(tus), "queue": queue, "collapsed": collapsed,
            "secs": round(time.time() - t0, 1), "cache_hits": v.hits, "cache_misses": v.misses,
            "restored": restored, "diag": diag,
            "trials": {r["tu"]: r.get("trial") for r in results if r.get("trial")},
            "collapse_errors": {r["tu"]: r["collapse_error"] for r in results if r.get("collapse_error")},
            "tidied": sum(r["tidied"] for r in results), "hoisted": sum(r["hoisted"] for r in results),
            "flagged": sum(r["flagged"] for r in results), "unflagged": sum(r["unflagged"] for r in results),
            "results": results}
