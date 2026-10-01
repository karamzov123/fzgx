# Finding 255 — fn_80071CC0 remaining-asm triage: convertible leaf set exhausted; frame-quirk blocks all callers

Unit: `main/game/fn_80071CC0`  (lease holder: `integ`)
Turn scope: one new evidence action per directive. After converting the two robust leaves
(`fn_800724C8` #252, `fn_80071D2C` #254) and triaging the frame-quirk wrappers (`fn_800723B8`/
`fn_800723D8` #253), this finding enumerates the remaining asm and why none is a fresh,
quirk-immune, single-edit convertible candidate.

## Already natural C at HEAD (solved, NOT new targets)
- fn_80071CE0 (u32 field accessor, 100.0)
- fn_800724C8 (4-byte blr leaf, #252 / reconfirmed)
- fn_80071D2C (4-byte blr leaf, #254 this lease)
- fn_800723F8 (store-0 wrapper, 100.0)

## Remaining asm (ALL are callers -> all have a prologue frame -> subject to the
## whole-TU frame-size quirk documented in #253; none is a no-frame leaf)
- ModelDVD_OpenFile   (bl DVDOpen,        frame 0x10)  -> 100.0 only in siblings-asm ref ctx (context-conditional, see D8)
- ModelDVD_ReadAsync   (bl DVDReadAsync,   frame 0x10)  -> same
- ModelDVD_CancelSync  (bl DVDCancelSync,  frame 0x10)  -> same
- fn_80071D30          (107 ins, complex store/init)   -> blocked by body+frame
- GXCompareVecDirty    (symbol_state=mispin)           -> HARD STOP, do not re-attempt
- fn_80072014          (41 ins)                        -> body/quirk
- Snd_SetOutputModeBit0(48 ins)                        -> body/quirk
- fn_80072168          (39 ins)                        -> body/quirk
- fn_800721FC          (calls fn_80015E18,GXSetProjectionv) -> body/quirk
- GXComputeDeltaRatio  (symbol_state=terminal)         -> HARD STOP
- GXProjCache_Save     (#251 body-confirmed, frame-quirk residual) -> not single-edit closable
- GXProjCache_Restore  (symbol_state=plateau)          -> HARD STOP
- fn_800723B8          (#253 frame-quirk, 49.75)        -> needs sibling-pair solve
- fn_800723D8          (#253 frame-quirk, context-cond) -> needs sibling-pair solve
- fn_80072404          (register s32 a,b; 26 ins)      -> register-arg + frame quirk
- GXLoadMtxArray       (register u32 a; symbol_state=plateau, frame 0x10) -> HARD STOP/plateau

## Conclusion (per directive: "no new candidate -> record evidence and advance the unit")
There is NO remaining asm function on this lease that is a no-frame leaf (the only class proven
robustly exact under the single-target-edit rule). Every remaining candidate either:
  (a) is already solved (natural C at HEAD), or
  (b) is HARD-STOPPED by durable symbol_state (mispin/terminal/plateau), or
  (c) hits the whole-TU prologue-frame quirk that cannot be closed by a single target-function edit.

Therefore a *genuinely new* evidence action (distinct source/context/hypothesis, not a retry of
unchanged source) is NOT available on this lease this turn. The robust-conversion leaf set is
exhausted.

## Recommended next step (outside single-edit rule; for worker handoff)
Convert the display-list + ModelDVD wrappers as a SIBLING PAIR (or apply a TU/flag adjustment
that fixes the 8-vs-16 byte frame emission), then let the integrator gate the resulting batch.
This is the only path that closes the frame-quirk blockers; it is not a single-target-edit action
and is recorded here rather than attempted as a false "new candidate".

## 2026-08-31 (natc integ — evidence action, GXProjCache_Save)
LEASE VERIFIED: main/game/fn_80071CC0 (NATC_WORKER=integ, rung=1). Fresh non-excluded
symbol = GXProjCache_Save (attempts 0/12; twin GXProjCache_Restore excluded via plateau).

ONE evidence action taken: bounded family of 3 complete-unit candidates copied from the
live source (src/game/fn_80071CC0.c), each with exactly ONE target-function edit —
GXProjCache_Save converted to natural C, varying one evidence class (memcpy size/addr rep):
  c1: memcpy(lbl_8019F024, lbl_8019F008, 0x1C);
  c2: memcpy(lbl_8019F024, lbl_8019F008, 28);
  c3: memcpy(&lbl_8019F024[0], &lbl_8019F008[0], 0x1C);
All carry // provenance: retail-disassembly:GFZE01:0x80071CC0 line.

EVAL (natc_codegen_search.py --candidate, live compiler GC/1.2.5n, no telemetry flags):
  c1 29.08% prologue ; c2 29.08% prologue ; c3 29.08% prologue ; no winner.
NOT a false positive (live src is asm, not 100%).

GROUNDED DIVERGENCE (natc_feedback vs retail build/GFZE01/obj/game/fn_80071CC0.o):
  target    : stwu r1,-0x10(r1); mflr r0; lis r3,@ha; lis r4,@ha; stw r0,0x14(r1); ...
  candidate : mflr r0; stw r0,0x4(r1); stwu r1,-0x8(r1); lis r3,@ha; lis r4,@ha; ...
=> frame-size/order class: retail = 16B frame (stw r0,0x14), candidate = 8B frame
   (stw r0,0x4). Same whole-TU prologue-frame quirk as #253/#255: compiler drives frame
   size off a sibling in the TU, not the local body. NOT closed by a single GXProjCache_Save
   edit, and its twin GXProjCache_Restore is excluded (plateau) so no sibling-edit lever.

CONCLUSION: GXProjCache_Save is the same non-fixable-by-single-edit class as #253/#255.
No duplicate/already_solved/mispin refusal (3 distinct src_sha, attempts 0->1 registered).
Per directive: do NOT retry unchanged source; advance the symbol.
Candidate scratch retained: /tmp/natc-search-gxprojcache-save/{c1,c2,c3}.c (no durable
registration — best% != 100, exact-artifact preflight not passed).

## 2026-08-31 (natc integ — 2nd evidence action, GXProjCache_Save, distinct class)
LEASE re-verified: main/game/fn_80071CC0 (integ, rung=1). Symbol still fresh (attempts 0;
prior families were scratch-only, not durable-counted — directive: stay on symbol, no advance).

1 bounded --discriminate probe run first (directive-permitted): swept 98 sibling units,
"swept: 0, mispin suspects: 0" — confirms compiler pin GC/1.2.5n authoritative, no mispin
confound. (Unrelated __init_data/__init_hardware siblings in sweep are OoE init, not this TU.)

2nd evidence action: NEW evidence class = source/target ADDRESS EXPRESSION (prior class was
memcpy size/decimal). Size fixed 0x1C. 3 complete-unit candidates, one target-fn edit each:
  c1: memcpy((void*)lbl_8019F024, (void*)lbl_8019F008, 0x1C);
  c2: memcpy(&lbl_8019F024[0],  &lbl_8019F008[0], 0x1C);   (dedup of prior c3, distinct src_sha)
  c3: memcpy((char*)lbl_8019F024, (char*)lbl_8019F008, 0x1C);
All carry // provenance: retail-disassembly:GFZE01:0x80071CC0.

EVAL (natc_codegen_search.py --candidate, GC/1.2.5n, no telemetry flags):
  c1 29.08% prologue ; c2 29.08% prologue ; c3 29.08% prologue ; no winner.
NOT false positive (live src asm).

CONCLUSION: across TWO distinct evidence classes (memcpy size/decimal rep; address cast form)
the score is invariant at 29.08% prologue — the blocker is the TU-driven 16B-vs-8B frame
emission (grounded last turn), independent of body-level edits. No single GXProjCache_Save
edit closes it; twin GXProjCache_Restore excluded (plateau). Same non-fixable-by-single-edit
class as #253/#255. No duplicate/already_solved/mispin refusal (3 distinct src_sha).
Per directive: do NOT retry unchanged source; advance the symbol.
Scratch retained: /tmp/natc-save-d2/{c1,c2,c3}.c (no durable registration, best% != 100).

## 2026-08-31 (natc integ — 3rd evidence action, GXProjCache_Save, distinct class)
LEASE re-verified: main/game/fn_80071CC0 (integ, rung=1). Symbol still fresh (attempts 0;
families scratch-only).

3rd evidence action: NEW evidence class = COPY PRIMITIVE (memmove vs memcpy). NOTE: c1/c3 in
this family were first drafted with fabricated GXProjCache type / GXMemCpy symbol — REJECTED
as non-retail (directive: retail function only). Replaced with real SDK memmove
(src/dolphin/msl/mem.c is a real symbol in this build); added `extern void* memmove(...)` to
each unit (real decl, not fabricated). 3 complete-unit candidates, one target-fn edit each:
  c1: memmove((void*)lbl_8019F024, (void*)lbl_8019F008, 0x1C);
  c2: memmove(lbl_8019F024, lbl_8019F008, 0x1C);
  c3: memmove(&lbl_8019F024[0], &lbl_8019F008[0], 0x1C);
All carry // provenance: retail-disassembly:GFZE01:0x80071CC0.

EVAL (natc_codegen_search.py --candidate, GC/1.2.5n, no telemetry flags):
  c1 28.69% prologue ; c2 28.69% prologue ; c3 28.69% prologue ; no winner.
(memmove scores LOWER than memcpy's 29.08% — confirms retail's bl target is memcpy, not memmove.
NOT false positive, live src asm.)

CROSS-CLASS SUMMARY (3 actions, 9 candidates, 3 distinct evidence classes):
  class A memcpy size/decimal   : 29.08% prologue (x3)
  class B memcpy address form   : 29.08% prologue (x3)
  class C memmove primitive     : 28.69% prologue (x3)
All invariant in the TU-driven 16B-vs-8B frame-emission class (grounded: retail stwu r1,-0x10;
stw r0,0x14 vs candidate stwu r1,-0x8; stw r0,0x4). Blocker is independent of body-level edits;
no single GXProjCache_Save edit closes it. Twin GXProjCache_Restore excluded (plateau).
Same non-fixable-by-single-edit frame-quirk class as #253/#255.
No duplicate/already_solved/mispin refusal across all 9 (9 distinct src_sha).
Per directive: do NOT retry unchanged source; advance the symbol.
Scratch retained: /tmp/natc-save-d3/{c1,c2,c3}.c (no durable registration, best% != 100).

## 2026-08-31 (natc integ — 4th evidence action, GXProjCache_Save, distinct class)
LEASE re-verified: main/game/fn_80071CC0 (integ, rung=1). Symbol still fresh (attempts 0).

4th evidence action: NEW evidence class = VOLATILE QUALIFIER (qualifier-driven frame emission).
3 complete-unit candidates, one target-fn edit each (retail-plausible, no fabricated symbols):
  c1: { volatile unsigned char* d = lbl_8019F024; memcpy((void*)d,(void*)lbl_8019F008,0x1C); }
  c2: memcpy((volatile void*)lbl_8019F024, (void*)lbl_8019F008, 0x1C);
  c3: memcpy((volatile void*)lbl_8019F024, (volatile void*)lbl_8019F008, 0x1C);
All carry // provenance: retail-disassembly:GFZE01:0x80071CC0.

EVAL (natc_codegen_search.py --candidate, GC/1.2.5n, no telemetry flags):
  c1 21.00% reloc_shape  (REGRESSED — volatile local ptr widened frame, changed reloc shape)
  c2 29.08% prologue
  c3 29.08% prologue
  no winner. NOT false positive (live src asm).

CONCLUSION: volatile does NOT close the frame class; c1 regressed (reloc_shape), c2/c3 still
prologue. 4 distinct evidence classes now exhausted on this symbol:
  A memcpy size/decimal 29.08 prologue | B memcpy addr form 29.08 prologue |
  C memmove primitive 28.69 prologue | D volatile qualifier 29.08/21.00 prologue/reloc_shape
All invariant in the TU-driven 16B-vs-8B frame-emission class (grounded real bytes). The
blocker is independent of every body-level edit tried; no single GXProjCache_Save edit closes
it. Twin GXProjCache_Restore excluded (plateau). Same non-fixable-by-single-edit frame-quirk
class as #253/#255.
No duplicate/already_solved/mispin refusal across all 12 candidates (12 distinct src_sha).
Per directive: do NOT retry unchanged source; advance the symbol.
Scratch retained: /tmp/natc-save-d4/{c1,c2,c3}.c (no durable registration, best% != 100).

## 2026-08-31 (natc integ — 5th evidence action, GXProjCache_Save, distinct class)
LEASE re-verified: main/game/fn_80071CC0 (integ, rung=1). Symbol still fresh (attempts 0).

5th evidence action: NEW evidence class = EXPLICIT LOOP COPY (removes the memcpy callee entirely,
changes reloc shape). 3 complete-unit candidates, one target-fn edit each (retail-plausible, no
fabricated symbols):
  c1: for(i=0;i<7;i++) ((u32*)lbl_8019F024)[i]=((u32*)lbl_8019F008)[i];
  c2: for(i=0;i<28;i++) lbl_8019F024[i]=lbl_8019F008[i];
  c3: u32*d=(u32*)lbl_8019F024; u32*s=(u32*)lbl_8019F008; for(i=0;i<7;i++) d[i]=s[i];
All carry // provenance: retail-disassembly:GFZE01:0x80071CC0.

EVAL (natc_codegen_search.py --candidate, GC/1.2.5n, no telemetry flags):
  c1 0.00% reloc_shape ; c2 0.00% reloc_shape ; c3 0.00% reloc_shape  (no winner)
DECISIVE: with the memcpy callee removed, reloc shape no longer matches retail AT ALL (0%).
This confirms retail GXProjCache_Save DEFINITIVELY calls memcpy (the only non-zero classes are
the memcpy/memmove variants at 29.08/28.69%). The 29.08% 'prologue' ceiling is the best any
single-edit body achieves, and it is the TU-driven 16B-vs-8B frame-emission quirk.

5-CLASS EXHAUSTION SUMMARY (15 candidates, no refusals, 15 distinct src_sha):
  A memcpy size/decimal 29.08 prologue | B memcpy addr form 29.08 prologue |
  C memmove primitive 28.69 prologue | D volatile qualifier 29.08/21.00 |
  E explicit loop    0.00 reloc_shape (rules out non-memcpy implementations)
CONCLUSION: GXProjCache_Save cannot be closed by any single-edit body variant. Retail is a
memcpy(28B) call; the only residual mismatch is the whole-TU 16B-vs-8B frame emission, which
needs a sibling-pair / TU-flag adjustment (worker handoff, #253/#255 class). Twin excluded.
No duplicate/already_solved/mispin refusal. Per directive: do NOT retry unchanged source.
Scratch retained: /tmp/natc-save-d5/{c1,c2,c3}.c (no durable registration, best% != 100).

## 2026-08-31 (natc integ — EXHAUSTION DOSSIER, GXProjCache_Save, blocker for handoff)
LEASE re-verified: main/game/fn_80071CC0 (integ, rung=1). Symbol still returned fresh by
--next (attempts 0; all families scratch-only, not durable-counted).

BLOCKER: GXProjCache_Save is NON-CLOSABLE by any single-edit body variant. 5 distinct evidence
classes / 15 candidates tested (no refusals, 15 distinct src_sha):
  A memcpy size/decimal : 29.08% prologue
  B memcpy address form : 29.08% prologue
  C memmove primitive   : 28.69% prologue
  D volatile qualifier  : 29.08% / 21.00% (prologue / reloc_shape)
  E explicit loop       : 0.00% reloc_shape  (proves retail calls memcpy; non-memcpy ruled out)
Residual mismatch is the TU-driven 16B-vs-8B frame emission (grounded real bytes: retail
`stwu r1,-0x10(r1); stw r0,0x14(r1)` vs candidate `stwu r1,-0x8(r1); stw r0,0x4(r1)`). This is
the SAME whole-TU prologue-frame quirk as #253/#255; the compiler drives frame size off a
sibling in the TU, not the local body.

WHY NO 6th FAMILY: a "struct-assignment" variant would require a named struct type for
lbl_8019F008/lbl_8019F024 — NONE exists in the build (grep of build/GFZE01/{asm,src} and src/
found no struct/cache type). Fabricating one violates "retail function only". All other
body-level hypotheses are exhausted. Per directive ("a new turn alone is not a new candidate;
do not retry unchanged source/context/compiler identity; do not repeat"), further single-edit
candidates on this symbol are prohibited repeats.

RESOLUTION PATH (worker handoff, not a single-edit candidate): convert GXProjCache_Save +
GXProjCache_Restore as a SIBLING PAIR, or apply the TU/flag adjustment that fixes the 8-vs-16B
frame emission for the whole TU (recorded in #253/#255). GXProjCache_Restore is currently
EXCLUDED via plateau, so the handoff must un-park it or adjust the TU frame for both.

DIRECTIVE COMPLIANCE: no candidate generated this turn (prohibited repeat). Evidence dossier
written. Awaiting: (a) a different symbol/context within the lease, or (b) release of this unit
for the sibling-pair / TU-flag handoff. I will not emit further GXProjCache_Save candidates.
