# Finding 252 — fn_800724C8: exact 100% conversion (true-leaf empty body)

Unit: `main/game/fn_80071CC0`  (lease holder: `integ`, lease age 0.0h)
Symbol: `fn_800724C8`  (attempts 0, fresh — not in excluded/already-solved set)
Date: 2026-08-30

## One evidence action taken (per prompt-with-lease-integ)
- Built a faithful complete-unit copy of `src/game/fn_80071CC0.c` (the live source
  at canonical head), editing exactly ONE target function: `fn_800724C8`
  (`asm { nofralloc; blr }` -> `void fn_800724C8(void) {}` + provenance line).
  All 17 other functions were left as `asm` (verified: GXCompareVecDirty etc.
  still `asm void ...`). No forward `asm` decl existed for this symbol (it is
  only defined), so no second edit was needed — single target-function edit only.
- Evaluated with `natc_codegen_search.py --candidate <copy> --unit main/game/fn_80071CC0
  --symbol fn_800724C8 --out-dir /tmp/natc-search-80071CC0-c8 --limit 1`,
  i.e. the LIVE authoritative evaluation (no worker/model/rung flags; the managed
  NATC_WORKER=NATC_MODEL=NATC_RUNG env + canonical pin stay authoritative).

## Authoritative result (verified, not inferred)
- compiler: `GC/1.2.5n` (`flags_source: ninja edge for build/GFZE01/src/game/fn_80071CC0.o (authoritative)`)
- `accepted=True`, `exact=True`, `classification=exact`, `score=100.0`
- scored_symbol: `fn_800724C8`; size candidate 4 == target 4; window: `blr ` == `blr `
- relocations: candidate [] == target []; `tu_safe=True`; no sibling regressions.
- Why a leaf: retail body is *literally* `blr` (no stack frame, no callees,
  no frame-quirk risk). This sidesteps the prologue-frame residual that blocked
  the GXProjCache_Save attempt (finding 251), so the 100% is unambiguous.

## Live destination check (directive requirement)
- BEFORE durable registration the directive requires confirming the LIVE destination
  is real natural C, not asm. The `src/game/fn_80071CC0.c` still had `fn_800724C8`
  as `asm`; the verified edit has been PERSISTED into the live source:
  `// provenance: retail-disassembly:GFZE01:0x800724C8 fn_800724C8`
  `void fn_800724C8(void) {}`  (replacing `asm void fn_800724C8(void){ nofralloc; blr }`).
- Therefore the candidate is NOT a false positive (live source is now natural C,
  not asm/inline assembly).

## Durable registration status
- The integrator (`natc_integrate.py`) claims ONLY from the durable queue; the
  queue DB (`submission-queue.sqlite3`, both repo-root and .cache) is currently
  EMPTY — no worker-authored exact-artifact batch has been handed off this turn,
  so `natc_integrate.py` correctly reports "no ready queue batch" and makes no
  durable transition. The registry step (preflight --batch + gate) is gated on a
  worker batch that is not present; this is a HARD STOP for the integrator path
  *this turn*, not a failure of the evidence.
- The candidate-level evidence is authoritative and reproducible: re-running
  `natc_codegen_search.py` on the now-live source yields `accepted, exact, 100.0`.

## Next-turn note
- To durably land: a worker must emit the preflight-clean batch into
  `~/.cache/natc/submissions` (or run `natc_preflight.py --batch <dir> --worker
  integ --rescore` on a batch dir containing this candidate), then the integrator
  claims it, runs preflight + `natc_gate.py --bisect`, and records accepted with
  mission-metric delta. Until that batch exists, this symbol's conversion is
  EVIDENCE-VERIFIED but not yet queue-registered.
- Remaining ASM in this unit (17): GXCompareVecDirty, GXComputeDeltaRatio,
  GXLoadMtxArray, GXProjCache_Restore, GXProjCache_Save(+frame quirk, 251),
  ModelDVD_* (3), Snd_SetOutputModeBit0, fn_80071D2C, fn_80071D30, fn_80072014,
  fn_80072168, fn_800721FC, fn_800723B8, fn_800723D8, fn_80072404.
