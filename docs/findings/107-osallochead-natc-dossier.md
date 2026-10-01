# OSAllocHead NATC conversion dossier — 2026-08-25 (natc-w-cli-1)

Unit: main/dolphin/os/OSAllocHead (11 fns, 1716 B). Baseline: all asm transcription.

## Result
9/11 functions exact natural-C under GC/1.3 (discriminated; objdiff.json pin
mwcc_233_163n/GC-1.2.5n CANNOT produce the retail stwu-first prologues —
findings/06 confirmed GC/1.2.5n emits mflr-first):
- EXACT: fn_80008DB4(asm kept), OSAllocHead_nop_stub(asm), OSAlloc, OSFree,
  OSSetCurrentHeap_thunk, OSCreateHeap_wrapper_A/B, OSDestroyHeap, fn_800090A4
  (asm kept; raw r13 displacements replaced with symbol refs -> sda21 relocs).
- PLATEAU: OSInitAlloc 82.4%, OSCreateHeap 92.4%.

## Key discoveries
1. This unit is a GC/1.3 build (prologue family B: stwu-first). The compiler
   discriminator (step 7) settled it in one sweep; 1.2.5n wastes every attempt.
2. Wrappers call `OSCreateHeap()` with NO argument setup (bl only) — reproduced
   by a prototype-less `extern int OSCreateHeap();` declaration.
3. fn_800090A4's asm body used literal r13 displacements (-0x7c7c(r13) etc.);
   replacing them with bare symbol operands restores sda21 relocations -> 100%.

## Plateau classes
- OSCreateHeap (92.4, regalloc): retail saves r29/r30 callee-saved (frame 0x20)
  holding rounded lo/hi; wibo+GC/1.3 assigns them volatile regs (frame 0x10).
  Source shapes tried: param copy before acquire, rounding before/after acquire,
  non-register locals — none move the allocator.
- OSInitAlloc (82.4, sched): loop over cells must reload gAssetBudgetB and
  lbl_801A6740 per iteration to avoid 8x unrolling; remaining delta is store
  order of lbl_801A6738/g_currentHeapHandle vs the aligned computation.

## Artifacts
Candidate: orig/natc-scratch/cand7.c (+ .o, diff JSONs cand*.json).
Attempt log: natc_runs.sqlite attempts table.
