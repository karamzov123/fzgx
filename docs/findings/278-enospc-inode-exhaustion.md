# 278 — The fleet died of ENOSPC, not a model failure

**Date:** 2026-10-02. **Symptom:** every fleet family failed at once with a
non-model error, `OSError: [Errno 28] No space left on device: .../verify.stop`.

## It was inodes, not disk

`df -h` showed 73G free on a 621G volume. `df -i` showed **0 free of 41,369,600
inodes (100% used)**, and `touch` failed with ENOSPC.

`.fzgx/fixup/sessions/` held **28.9M files** across 841 per-function session
directories. `tools/fzgx/fixup.py:98` writes one content-addressed
`sources/<identity[:24]>.c` per candidate repair body, plus one compiled object
under `objects/`, for *every compile attempt*. That cache was never pruned.
Nothing else on the box was comparable (`.cache` 156k files, `build/` 28k).

## What is and is not irreplaceable

The corpus is **regenerable**, and this is the correction to the initial
read:

- `fixup.py:98-102` does `if not path.exists(): path.write_text(body)`. A
  **missing** source self-heals on the next pass. The `ValueError('changed
  content-addressed source')` fires only when a file *exists* with different
  content, so deletion is the safe direction.
- Nothing outside a session directory references those paths. Seeds are rebuilt
  from the ledger and corpus (`load_records`), not from `sessions/*/sources`.
  `ledger.db` contained **zero** `fixup/sessions` references.
- Only the *actionable* rows are read again: `--apply`, `--archive` and
  `repair_recipe` walk `best`, `frontier`, `matched`, and every ancestor chain.

So the collector (`tools/fzgx/fixup_gc.py`) keeps exactly those and drops the
rest, along with their `cache.json` rows and objects. Note sources are keyed by
`identity[:24]` while objects and `cache.json` key on the full 64-char id --
matching on the wrong one silently deletes everything.

**Freed:** 19,697,723 sources + 17,830,249 objects = **38.0M inodes**
(0 -> 38,005,682 free). Disk also improved to 288G free.

**Verified after the real run:** 3,035 `repair_recipe` calls across all 841
sessions with 0 errors; 1,093 actionable sources hash-verified with 0 missing
and 0 mismatches; `archive_sources` clean; a second GC pass frees 0
(idempotent); ledger unchanged (5,548 matched, 20,406 attempts, $368.92,
`integrity_check ok`).

## Prevention

`orchestrate.py` now checks free inodes at startup and runs the collector when
they are low, refusing to start rather than failing every agent into ENOSPC.
The supervisor had been crash-looping into `start-limit-hit` on exactly this
error (`status-v3.json.<pid>.tmp`), which is what made the fleet look like a
provider failure.

## Two things that were *not* broken

- **`cost_usd` freezing at $0** for opencode/cline is the documented design
  (`docs/findings/276`): those providers report no cost and no public rate
  exists, so a dollar figure would be invented. The cache split is recorded
  instead. Not a bug.
- The models never crashed. The single non-model error was the kernel refusing
  to create a file.

## Operating note

`tools/fleet.py daemon` is only a shim: it imports and runs
`fleet_multi.main()` (`fleet.py:479-480`). The systemd unit
`fzgx-fleet.service` therefore runs the correct multi-family supervisor; it is
enabled at boot and `Linger=yes`, so it survives logout and reboot.
Restart-on-failure was verified by `kill -9` of the supervisor PID: systemd
tore down the tree, waited `RestartSec=60`, and resumed all four families.