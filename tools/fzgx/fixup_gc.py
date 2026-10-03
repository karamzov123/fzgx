"""Scratch collector for the fixup session corpus.

Every compile attempt writes one content-addressed ``sources/<id>.c`` and one
``objects/<group>/<id>/<id>.o``. A long search accumulates millions of them --
28.9M files against a 41.4M inode budget, which is how a box reaches
``ENOSPC`` with 73G of disk free.

Nothing outside a session directory points at those paths: seeds are rebuilt
from the ledger and the corpus, and :meth:`Engine.record` rewrites a source
whose file is missing, so an absent file self-heals on the next pass. Only the
*actionable* rows need to survive -- the ones ``--apply``, ``--archive`` and the
next round read through :func:`repair_recipe`: every ``best``, every
``frontier`` entry, every ``matched`` row, the highest ``raw_percent`` row per
symbol, and the full ancestor chain of each (a recipe replays parent bodies).

    python -m tools.fzgx.fixup_gc            # report, then collect
    python -m tools.fzgx.fixup_gc --dry-run  # report only
    python -m tools.fzgx.fixup_gc --root DIR  # a corpus other than .fzgx/fixup/sessions
"""
from __future__ import annotations

import argparse
import json
import os
import shutil
import sys
from pathlib import Path

STATE_DIR = Path(os.environ.get("FZGX_STATE", Path(__file__).resolve().parents[2] / ".fzgx"))


def actionable(report: dict) -> set[str]:
    """Ids a later pass still needs: results, survivors and their recipes."""
    records = report.get("records") or []
    by_id = {r["id"]: r for r in records if r.get("id")}
    keep: set[str] = set()

    def hold(row: dict | None) -> None:
        """Keep a row and every ancestor its replay recipe walks through."""
        stack = [row["id"]] if row and row.get("id") in by_id else []
        while stack:
            current = stack.pop()
            if current in keep:
                continue
            keep.add(current)
            parent = by_id.get(current, {}).get("parent")
            if parent and parent in by_id:
                stack.append(parent)

    for row in (report.get("best") or {}).values():
        hold(row)
    for group in (report.get("frontier") or {}).values():
        for row in group:
            hold(row)
    for row in records:
        if row.get("matched") or row.get("link_rejected"):
            hold(row)

    # The word frontier can prefer fewer differing instructions to a higher
    # objdiff percentage; both are archived for the next pass.
    raw: dict[str, dict] = {}
    for row in records:
        if row.get("object") and "raw_percent" in row:
            if row["raw_percent"] > raw.get(row.get("symbol", ""), {}).get("raw_percent", -1):
                raw[row.get("symbol", "")] = row
    for row in raw.values():
        hold(row)
    return keep


def sweep(session: Path, dry_run: bool) -> tuple[int, int, int]:
    """Drop unreachable sources, their objects and their cache rows."""
    report_path = session / "report.json"
    keep = actionable(json.loads(report_path.read_text())) if report_path.exists() else set()
    cache_path = session / "cache.json"
    cache = json.loads(cache_path.read_text()) if cache_path.exists() else {}

    dropped_sources = dropped_objects = dropped_cache = 0
    sources = session / "sources"
    if sources.is_dir():
        # fixup.record names a source identity[:24] + '.c', while objects/ and
        # cache.json key on the full 64-character id.
        keep_stems = {ident[:24] for ident in keep}
        for entry in sources.iterdir():
            if entry.suffix == ".c" and entry.stem in keep_stems:
                continue
            dropped_sources += 1
            if not dry_run:
                entry.unlink(missing_ok=True)

    # objects/<group>/<id>/<id>.o -- the id is the directory name.
    objects = session / "objects"
    if objects.is_dir():
        for group in objects.iterdir():
            if not group.is_dir():
                continue
            for ident in group.iterdir():
                if ident.name in keep:
                    continue
                dropped_objects += sum(len(f) for _, _, f in os.walk(ident))
                if not dry_run:
                    shutil.rmtree(ident, ignore_errors=True)

    if cache and not dry_run:
        cache = {k: v for k, v in cache.items() if k in keep}
        dropped_cache = 1
        cache_path.write_text(json.dumps(cache))

    if not dry_run:
        for parent in (sources, objects):
            if parent.is_dir():
                for stale in sorted(parent.rglob("*"), reverse=True):
                    if stale.is_dir():
                        stale.rmdir()  # only succeeds when empty
                parent.rmdir() if not any(parent.iterdir()) else None
    return dropped_sources, dropped_objects, dropped_cache


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--root", type=Path, default=STATE_DIR / "fixup" / "sessions")
    parser.add_argument("--dry-run", action="store_true", help="report without deleting")
    args = parser.parse_args(argv)

    if not args.root.is_dir():
        print(f"no session corpus at {args.root}")
        return 1

    sessions = sorted(p for p in args.root.iterdir() if p.is_dir())
    total = [0, 0, 0]
    for index, session in enumerate(sessions, 1):
        counts = sweep(session, args.dry_run)
        total = [a + b for a, b in zip(total, counts)]
        if index % 50 == 0 or index == len(sessions):
            verb = "would free" if args.dry_run else "freed"
            print(
                f"[{index}/{len(sessions)}] {verb} {total[0]:,} sources, "
                f"{total[1]:,} objects, {total[2]} cache files",
                flush=True,
            )
    print(f"sources={total[0]} objects={total[1]} cache_files={total[2]} dry_run={args.dry_run}")
    return 0


if __name__ == "__main__":
    sys.exit(main())