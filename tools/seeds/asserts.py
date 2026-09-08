#!/usr/bin/env python3
"""Harvest assert calls: (function, file, source line, message) from a module's disassembly.

The game's assert helper (`fn_8000C49C` in main.dol) is called as
`assert_fail(__FILE__, __LINE__, message)`. Every call therefore pins the calling
function to a source file and a line number, and the message often names the
global being checked ("game_camera is NULL at set shake"). Output:
state/seeds/asserts_<module>.json, plus a comparison against tus.json.

Usage: uv run tools/seeds/asserts.py [--module main_rel] [--assert-fn fn_8000C49C]
"""

from __future__ import annotations

import argparse
import collections
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from fzgx.project import ROOT, Project  # noqa: E402  (scoped: script under tools/seeds, package under tools/)


def harvest(p: Project, module: str, assert_fn: str):
    calls = []
    for fn in p.function_asm(module).values():
        regs = {}
        for l in fn.asm:
            if ": " not in l:
                continue
            ins = l.split(": ", 1)[1]
            m = re.match(r"^(?:addi|li) r([345]), (?:r\d+, )?(.+)$", ins)
            if m:
                regs[m.group(1)] = m.group(2)
            if ins.startswith(f"bl {assert_fn}"):
                file_sym = re.sub(r"@l$", "", regs.get("3", ""))
                msg_sym = re.sub(r"@l$", "", regs.get("5", ""))
                try:
                    line = int(regs.get("4", ""), 0)
                except ValueError:
                    line = None
                file = p.string_at(module, file_sym) if file_sym.startswith("lbl_") else None
                if not file or not file.endswith(".c") or line is None:
                    continue
                msg = p.string_at(module, msg_sym) if msg_sym.startswith("lbl_") else None
                calls.append({"fn": fn.symbol.name, "addr": fn.symbol.addr, "file": file, "line": line,
                              "msg": (msg or "").rstrip("\n")})
    return sorted(calls, key=lambda c: c["addr"])


def main(argv=None) -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--module", default="main_rel")
    ap.add_argument("--assert-fn", default="fn_8000C49C")
    a = ap.parse_args(argv)
    p = Project()
    calls = harvest(p, a.module, a.assert_fn)
    out = ROOT / "state" / "seeds" / f"asserts_{a.module}.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(calls, indent=1) + "\n")
    by_file = collections.Counter(c["file"] for c in calls)
    print(f"{len(calls)} asserts in {len({c['fn'] for c in calls})} functions -> {out.relative_to(ROOT)}")
    print("by file:", by_file.most_common(12))
    tus_path = p.module_config_dir(a.module) / "tus.json"
    if tus_path.exists():
        owner = {f: t["file"] for t in json.loads(tus_path.read_text())["tus"] for f in t["functions"]}
        dis = [(c["fn"], c["file"], owner.get(c["fn"])) for c in calls if owner.get(c["fn"]) != c["file"]]
        print(f"TU map disagreements: {len(dis)}", dis[:8])
    return 0


if __name__ == "__main__":
    sys.exit(main())
