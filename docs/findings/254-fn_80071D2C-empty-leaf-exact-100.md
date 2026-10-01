# Finding 254 — fn_80071D2C: EXACT 100% CONVERSION (4-byte `blr` leaf, durable)

Unit: `main/game/fn_80071CC0`  (lease holder: `integ`)
Symbol: `fn_80071D2C`  (retail: 4-byte `.text`, single `blr`; no call, no frame, no relocations)
Evidence class: true-leaf empty `void(void)` body — single target-function edit.
Context: live at-HEAD TU (where `fn_800724C8` is already natural C; all other fns asm).

## Authoritative result (natc_codegen_search.py --candidate, no telemetry/identity flags)
- compiler: `GC/1.2.5n` (canonical head `3592693cf89e2474855da805ca9b8d9f239509b5`, flags_source "ninja edge for build/GFZE01/src/game/fn_80071CC0.o (authoritative)", incl `-DDEBUG=1`, `cache_hit=False`, `elapsed_ms=80`)
- `accepted=True, reason=accepted, classification=exact, exact=True, score=100.0`
- `window[0] = {instruction:0, candidate:'blr', target:'blr'}` — size 4->4
- `frame_size: {candidate:None, target:None}` (NO prologue frame generated — immune to the frame quirk)
- `relocations: {candidate:[], target:[]}` (no R_PPC_* — confirms no data/code relocations)
- `tu_safe=True`, and the whole-TU scan shows `fn_80071D2C` 100.0 with `fn_800724C8` already natural C (i.e. NOT context-conditional, unlike fn_800723D8/fn_800723B8 in #253).

## Live-destination guard (directive rule)
- Live `src/game/fn_80071CC0.c` had `fn_80071D2C` as `asm void fn_80071D2C(void){ nofralloc; blr; }` at HEAD.
- The 100% is genuine natural C (empty `void fn_80071D2C(void){}`), NOT asm. No false-positive risk.
- Re-checked the persisted live-edited copy via natc_codegen_search: `accepted=True, reason=accepted`
  (winner-short mode omits the inline `evaluation` block, but the acceptance verdict matches the
  winning candidate run which showed `exact=True, score=100.0`).

## Durable edit
- Applied to `src/game/fn_80071CC0.c` (live destination is now real natural C):
  ```
  // provenance: retail-disassembly:GFZE01:0x80071D2C fn_80071D2C
  void fn_80071D2C(void)
  {
  }
  ```
- Provenance line present (symbol-level, from matched `// provenance` seed).

## Why this is durable (vs #253's trap)
The display-list wrappers (fn_800723B8/23D8) hit a context-conditional prologue-frame quirk
(8-byte vs 16-byte frame chosen by whole-TU sibling state), so their 100% was a reference-context
artifact. fn_80071D2C is a 4-byte `blr` leaf: it emits NO frame, so there is nothing for the frame
quirk to affect. Exact match is robust.

## Candidate artifact (for worker/integrator handoff)
/tmp/natc-search-80071CC0-d2c/C_fn_80071D2C_empty.c  (faithful copy, one target-function edit)

## Integrator / queue
No ready queue batch this turn (submission-queue.sqlite3 empty) — the integrator cleanly STOPS with
`no ready queue batch`. The durable conversion is recorded in the live source + this dossier;
it will surface to the integrator once a worker emits the matching preflight-clean batch, or on the
next NATC delivery sweep.
