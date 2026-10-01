# PLATEAU DOSSIER — MTXFused: fn_8006D3D0 + fn_8006D46C
Worker: oxalpha1 · 2026-08-25 · unit main/dolphin/mtx/MTXFused (compiler GC/1.2.5n per objdiff.json; discriminator showed 1.3 family scores ~15pp higher on identical source — likely WRONG-COMPILER symptom, see below)

## Status
Not converted. Best natural-C candidate: fn_8006D3D0 81.2% (156B target vs 168B),
fn_8006D46C 39.6% (312B vs 228B). All prior asm bodies remain intact and exact;
unit untouched, gate unaffected.

## Facts established (verified, reusable)
- sdata2 constants via r2=0x801AEE40 (NOT the 0x800FEE40 in the strategy doc):
  -0x7a88 dbl 1.0 · -0x7a80 f32 0.0f · -0x7a7c f32 1.0f · -0x7a78 f32 FLT_EPSILON.
- `__fabs` intrinsic emits the `fabs` instruction directly (fabsf libcall does not).
- Prologue shape stwu/mflr/stw/stfd f31,0x10/psq_st f31,0x18 reproduced exactly by
  a plain f32 local copy under compiler 1.3.
- Compiler discriminator (step 7): same source scored 40.2% on 1.2.5n vs 55.9% on
  1.3 — objdiff.json says mwcc_233_163n for this unit but flags match 1.3-family
  output better. Recommend integrator verify unit compiler assignment.

## Remaining deltas on best candidate (cand33/cand26 line)
1. Retail sign test: `fcmpo cr0,f31,f0; ble ->sqrt-path`; every C shape tried
   gives either ble->+0x4000 or an extra cror eq,lt,eq. ~8 instrs.
2. Retail eps compare emits plain `ble` with NO cror eq,lt,eq; all our shapes
   emit cror (MWCC >=/< lowering). ~2 instrs.
3. Retail atanf path is `bl atanf; b epilogue` with NO fctiwz conversion, while
   li r3,±0x4000 integer constants exist on other paths. No pure-C return type
   reproduces both (s16/s32 → fctiwz block appears; float → constants become lfs).
   This smells like the original used a non-standard declaration or inline asm
   tail. ~6 instrs.

## Attempt log (33 compile+diff cycles, ~7ms each)
Shapes tried: fabsf libcall, static union fabs, __fabs; int/s32/float returns;
sign-test order ×4; eps polarity ×4; sqrtf arg as double/float/temp/fnmsubs;
single-return wrap vs early returns; register params; compilers 1.2.5(n)/1.3/
1.3.2(r). Scores: 20→30→34→40→55(1.3)→74→78→81 plateau over last ~10 attempts.

## Recommendation
Escalate to paid model with this dossier, or resolve the compiler-version
question first (T19/T11): if the true compiler is 1.3, several deltas may vanish.
Candidate sources preserved in ~/.cache/natc/work/cand*.c (best: cand33.c,
cand26.c float-return variant). Tooling in ~/.cache/natc/work/{build_diff,score,
score13,window,full}.py is unit-generic except paths.
