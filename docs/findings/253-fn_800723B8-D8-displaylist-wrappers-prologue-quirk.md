# Finding 253 — fn_800723B8 / fn_800723D8: display-list wrappers blocked by context-conditional prologue frame quirk

Unit: `main/game/fn_80071CC0`  (lease holder: `integ`)
Evidence class: single-call display-list wrappers (`GXBeginDisplayList` / `GXEndDisplayList`), one TU context.
Authority: `GC/1.2.5n` (ninja-edge flags for `build/GFZE01/src/game/fn_80071CC0.o`), canonical head `3592693c…` / context `e07052a3…`.
Tool: `natc_codegen_search.py --candidate` (bounded family, 2 complete-unit candidates, `--out-dir /tmp`).

## What was tested (every candidate = faithful copy of live source, exactly ONE target-function edit, provenance line present)

- `C_fn_800723B8_wrapper.c` — `fn_800723B8` asm → `void fn_800723B8(void){ GXBeginDisplayList(); }`
- `C_fn_800723D8_wrapper.c` — `fn_800723D8` asm → `void fn_800723D8(void){ GXEndDisplayList(); }`
  (the B8 copy kept D8 as asm; the D8 copy kept B8 as asm — i.e. the *reference* context where the tested symbol's siblings are asm.)

## Results (authoritative, reproducible)

| symbol | candidate body | score | accepted | primary_class | frame (cand/tgt) |
|--------|----------------|-------|----------|---------------|------------------|
| fn_800723B8 | `GXBeginDisplayList();` | 49.75 | No | prologue | 8 / 16 |
| fn_800723D8 | `GXEndDisplayList();` | **100.0** | **Yes (winner)** | exact | 16 / 16 |

For `fn_800723D8` in the reference (siblings-asm) context the full 8-instruction window is byte-identical:
`stwu r1,-0x10(r1) / mflr r0 / stw r0,0x14(r1) / bl GXEndDisplayList / lwz r0,0x14(r1) / mtlr r0 / addi r1,r1,0x10 / blr`, relocation `R_PPC_REL24`, size 32/32, `tu_safe: True`.

## CRITICAL false-positive trap caught (directive rule: a 100% while live source is asm/changed is suspect)

`fn_800723D8`'s 100% is **context-conditional**. When the candidate TU has the tested symbol as natural C but its siblings remain `asm`, the compiler emits a 0x10 frame and matches. When the unit already contains a natural-C sibling at HEAD (specifically `fn_800724C8` is natural C at HEAD), re-running the *same* `fn_800723D8` body in the live context drops to **49.75, prologue**, frame 8/16 — because the compiler's frame decision depends on the whole-TU sibling set, not the target body alone.

So `fn_800723D8`'s 100% does NOT represent a durable live conversion under the current (partially-converted) TU. It is recorded as **candidate-context evidence only**, not a confirmed solve.

## Blocking mechanism (shared with fn_800723B8 and earlier GXProjCache_Save #251)

The divergence is purely the prologue frame reservation: candidate emits `stwu r1,-0x8(r1)` + `stw r0,0x4(r1)` (8-byte frame, LR at 0x4) where retail requires `stwu r1,-0x10(r1)` + `stw r0,0x14(r1)` (16-byte, LR at 0x14). The actual `bl <callee>` and epilogue (`lwz r0 / mtlr / addi r1 / blr`) match. MWCC 1.2.5n sets this frame size from TU-level call-frame handling, so it is **not controllable by a single target-function edit** per the strict one-edit rule. This is the same class that produced the `GXLoadMtxArray` plateau.

## No durable src/ edit applied

Because the conversion is not confirmed in the live (HEAD) context, per directive (inspect LIVE destination; 100%-in-candidate ≠ live conversion) NO `src/game/fn_80071CC0.c` edit was committed. The live source remains at HEAD: `fn_800724C8` already natural C (prior committed solve); `fn_800723B8`, `fn_800723D8`, and the four blocked symbols remain `asm`. The working tree is clean (`git status` clean for the unit file).

## Integrator / queue state (verified this turn)

- `natc_integrate.py --worker integ` → `integrator: no ready queue batch` (clean HARD STOP — no worker-authored batch present).
- Both `submission-queue.sqlite3` DBs have empty tables → no ready, unclaimed batch to deliver.
- Durable `symbol_states` for this unit has 4 rows, all blocked: `GXCompareVecDirty`=mispin, `GXComputeDeltaRatio`=terminal, `GXLoadMtxArray`=plateau, `GXProjCache_Restore`=plateau. `fn_800723B8`/`fn_800723D8` are NOT in symbol_states (fresh, never marked).

## Recommended next (out of band for the worker, not this turn's action)

The prologue-frame class is systematic for wrappers in this TU and is gated by whole-TU sibling state. A durable solve likely requires converting the wrapper **together with** its frame-affecting siblings in one batch, or a compiler-flag/context adjustment — outside the single-target-edit rule. Suggest escalating `fn_800723B8`+`fn_800723D8` as a sibling pair (evidence class: "display-list wrappers, convert as batch") rather than individually.
