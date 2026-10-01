# Stranded-pool harvest probe — 2026-08-27 (fleet/tool, supervisor tool-health mandate)

Run by tooling worker (`tool`) as a follow-up to the durable `health-state.json`
alert seen in cycle 1 (`stranded=244`, `last_alert_key=stranded-work`). The
signal was never investigated until now.

## What "stranded" means
`~/.cache/natc/health-state.json` is written by a monitor OUTSIDE this repo
(grep for `health_state|stranded_recoverable|last_alert_key` across `tools/`
and the tree returns 0 — it lives only in `.hermes` session state, i.e. an
agent/supervisor-side writer). The number maps to `tools/natc_harvest.py`'s
purpose: **conversions that scored 100% but never landed** (lost to packaging —
partial-file candidates, sub-minimum batches, expired leases, moved baselines).
`stranded_recoverable` = those whose source survives via `attempts.src_sha` +
`~/.cache/natc/scratch/` candidates.

## Live action (read-only on src/ — integ-safe, harvest already ratified 77aba4a)
```
uv run --with capstone python3 tools/natc_harvest.py \
    --emit ~/.cache/natc/scratch/tool/harvest-run-0827
```
Harvest writes ONLY to its `--emit` dir; it does NOT touch `src/`, does NOT
modify `natc_gate` refusal logic, does NOT delete `.gateorig`.

## Result (fresh, this run)
- scored 100% and still asm in src/: **248**
- **PACKAGEABLE (full preflight clean): 3**  -> emitted 1 batch
- needs re-conversion (preflight fails): 80
- source recovered by src_sha: 83
- source no longer on disk: 71
- 100%-but-still-asm (false positive, NOT a conversion): 94

Emitted batch (gate-ready, NOT landed — integ still gates):
`~/.cache/natc/scratch/tool/harvest-run-0827/GXGeometry/`
- `GXGeometry.c` line 1: `// dest: src/dolphin/gx/GXGeometry.c`
- 3 natural-C bodies: `GXInitSpecularDir`, `__GXInitSpecularDirZ`,
  `GXInitLightAttnCoefs` (rest remain `asm` — correct partial batch)
- `CARD.md` present.

## Why this is NOT a tool worker action to finish
- Contract L143: tool worker may NOT self-sign / land. The batch is a
  **proposed** delivery; the integrator (`integ`) runs `natc_gate.py` to
  actually land it.
- `health-state.json` durable count (`stranded=244`) has NOT dropped because
  this was a probe emit, not a landing. The 244 ≈ the 248 still-asm figure;
  `stranded_recoverable=80` ≈ the 80 needing re-conversion / 83 src-recovered.

## Recommendation to next actor (integ)
Gate the emitted `GXGeometry` batch via `natc_gate.py`. If it passes, land it;
that single action should drop `stranded` by ~3 and is the only fleet-wide
effect available from this probe. The remaining 80 re-conversion-needed and
71 source-lost functions require worker re-conversion (out of tool-worker
scope per L13).

## Tool-health conclusion
`natc_harvest.py` is functional end-to-end against the live durable queue: it
correctly separates packageable / re-conversion-needed / source-lost /
false-positive, and refuses to emit an empty or partial artifact. The durable
`stranded-work` alert is a real fleet signal (248 stranded functions) but is an
**integration-domain** problem (packaging/landing), not a tool defect. Tooling
worker has produced the only deliverable it can: a verified, gate-ready batch
for integ.
