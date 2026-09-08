"""Exemplars of fixes: the (before, after) pairs of bodies where one check plateaued and the
next check matched, with the rows that changed. Mined from the per-check history that
`api.check` keeps (.fzgx/checks/<key>/NNN.c + index.jsonl). The context shows an agent the
nearest exemplars for its failure mode: the same kind of rows, and the edit that closed them.
"""

from __future__ import annotations

import difflib
import json
from pathlib import Path
from typing import Dict, List, Optional

from . import oracle, stuck
from .project import STATE_DIR, Project


def mine(p: Project, max_diff_lines: int = 40) -> List[Dict[str, object]]:
    out: List[Dict[str, object]] = []
    root = STATE_DIR / "checks"
    if not root.is_dir():
        return out
    for d in sorted(root.iterdir()):
        idx = d / "index.jsonl"
        if not idx.exists():
            continue
        recs = [json.loads(l) for l in idx.read_text().splitlines() if l.strip()]
        for prev, cur in zip(recs, recs[1:]):
            if cur.get("matched") and prev.get("ok") and not prev.get("matched") and (prev.get("percent") or 0) >= 60:
                a = (d / f"{prev['n']:03d}.c"); b = (d / f"{cur['n']:03d}.c")
                if not (a.exists() and b.exists()):
                    continue
                at, bt = a.read_text(), b.read_text()
                diff = [l for l in difflib.unified_diff(at.splitlines(), bt.splitlines(), lineterm="", n=1) if not l.startswith(("---", "+++"))]
                if not diff or len(diff) > max_diff_lines:
                    continue
                key = d.name.replace("__", ":")
                sym = p.resolve(key)
                mode, kinds = None, {}
                if sym is not None and (p.target_object_for(sym) or True):
                    # the failure mode of the plateaued body, from its object diff
                    try:
                        res = oracle.check(p, key, 0, source=a)
                        if res.ok:
                            lrows, rrows = res._rows
                            kinds = stuck.classify_rows(lrows, rrows); mode = stuck._pure(kinds, lrows, rrows)
                    except Exception:
                        pass
                out.append({"symbol": key, "from": prev.get("percent"), "mode": mode, "kinds": {k: v for k, v in kinds.items() if ":" not in k},
                            "diff": diff, "lines": len(diff)})
    (STATE_DIR / "exemplars.json").write_text(json.dumps(out, indent=1))
    return out


def nearest(mode: Optional[str], kinds: Dict[str, int], limit: int = 2) -> List[Dict[str, object]]:
    """Exemplars whose plateau looked like this one (same pure mode first, then shared kinds)."""
    path = STATE_DIR / "exemplars.json"
    if not path.exists():
        return []
    try:
        ex = json.loads(path.read_text())
    except ValueError:
        return []
    def score(e):
        s_ = 3 if mode and e.get("mode") == mode else 0
        s_ += len(set(kinds) & set(e.get("kinds") or {}))
        return (-s_, e.get("lines", 99))
    ranked = sorted((e for e in ex if e.get("diff")), key=score)
    return [e for e in ranked if score(e)[0] < 0][:limit]
