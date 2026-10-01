# 254 — item 2 post-fix verification (8th pass, tool worker)

Date: 2026-08-26
Branch: fleet/tool, HEAD bf5e247 (clean)

## Why this pass was NOT theater

The skill's re-dispatch rule says to skip self-tests on identical bytes. Bytes
were NOT identical this cycle: c23e349 changed the SDA21 displacement bucketing
in the `psx_dis` normaliser that item 2 (`similar.py`) consumes, and bf5e247
documented that bug as finding 253. A normaliser change is exactly the class of
change that can silently regress retrieval, so the full battery was re-run.

## Results

- 4/4 `--self-test` rc=0:
  - find_xrefs: victim=OSRegisterVersion, callers=14, frag lines=2 (cache hit)
  - similar: 2235 fns, probe=GXSetMisc, top=GXClearVtxDesc@0.050, 1 accepted twin
  - emit_m2c_asm: 50 lines, round-trip byte-identical
  - natc_loop: context 3189 chars, uncarved-guard=unit_uncarved fires
- Fixture suite: **45 passed in 10.76s** (was 42 — the SDA21 fix shipped 3 new
  regression fixtures, which is the correct response to finding 253).

## Retrieval quality confirmed improved (the point of the fix)

`similar.py --symbol CARDRead --k 5`:

    1.000  CARDWrite      main/dolphin/card/CARDWrite  (72 B)
    0.684  CARDSetStatus  main/dolphin/card/CARDStat
    0.684  CARDRename     main/dolphin/card/CARDRename
    0.684  CARDMount      main/dolphin/card/CARDDir
    0.684  CARDCreate     main/dolphin/card/CARDCreate

The exact CARDWrite twin at J=1.000 is now recovered. Finding 251 recorded that
pre-fix retrieval topped out at 0.684 for this probe (the bug zeroed the SDA21
displacement class, collapsing distinct sda21 accesses into one token and
flattening near-identical thunks). This is the corrected behaviour, not an
environment artifact.

## find_xrefs real-input spot check (ADDR16 array + same-TU suppression)

`--cslice OSPanic`: 15 callers, 5 callees, global
`OSErrorFmt_80122C30 (22 B, .data)` via ADDR16_HA+LO, histogram
`{ADDR16_LO:1, ADDR16_HA:1}`, and the minimal fragment emits
`extern unsigned char OSErrorFmt_80122C30[22];  /* shape unknown */` — the
documented expected shape.

## Compliance

- on_main = 0 (four tools not yet promoted — correct, ratify pending)
- `git status --porcelain` empty; the finding-252 src/ carve regression files are
  no longer present in the worktree and remain preserved at
  `~/.cache/natc/scratch/tool/src-writeback-evidence-20260826/`
- `units.worker='tool'` = 0 — no lease held
- `natc_gate.py` refusal logic untouched; no `.gateorig` deleted
- No ninja, no DOL build, no commit to main

## Blocker (unchanged, and not mine to clear)

`sed -n '342p' tools/REVIEW-queue-1-4.md` still returns the literal `<date>`
template, so items 1-4 are **UNRATIFIED**. The release rule requires the
hard-tail worker's review before fleet-wide promotion; the tooling worker cannot
self-ratify. Queue items 1-4 are spec-complete, green, and idle pending that
sign-off.

Next actor: **hard-tail** (ratify or NACK), then **integ** (promote).
Re-dispatching `tool` cannot advance this.
