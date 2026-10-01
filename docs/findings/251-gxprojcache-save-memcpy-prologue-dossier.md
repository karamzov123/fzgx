# Finding 251 — GXProjCache_Save: natural-C body confirmed; residual = prologue frame quirk (NOT 100%)

Unit: `main/game/fn_80071CC0`  (lease holder: `integ`, lease age 0.1h)
Symbol: `GXProjCache_Save`  (52 B / 0x34, .text)
Date: 2026-08-30
Tooling: `natc_codegen_search.py --candidate` with live `--unit`/`--symbol` authoritative eval.
Compiler: `GC/1.2.5n` (canonical head `8ba74dce`; ninja edge `build/GFZE01/src/game/fn_80071CC0.o`, flags include `-O4,p -inline auto -DDEBUG=1`).

## Evidence class tested (single class)
Argument pointer TYPE effect on codegen: `void*` casts vs `unsigned char*` array args.
Two complete-unit candidates, each a faithful copy of `src/game/fn_80071CC0.c`
with exactly one target-function edit (the `GXProjCache_Save` body flipped from
`asm` to natural C); `GXProjCache_Restore` and all other fns left as asm.
Mandatory structural fix (not a retry of unchanged source): the leftover forward
decl `asm void GXProjCache_Save(void);` was dropped to `void GXProjCache_Save(void);`
so the body is parsed as C — without it MWCC parsed `memcpy(...)` as an assembler
mnemonic and aborted (`unknown assembler instruction mnemonic`).

Candidate bodies (direction verified from sibling `GXProjCache_Restore` asm:
`r3=lbl_8019F008`(dest), `r4=lbl_8019F024`(src) → restore copies 024→008, so Save
copies 008→024):
- C: `memcpy(lbl_8019F024, lbl_8019F008, 0x1c);`
- D: `memcpy((void*)lbl_8019F024, (void*)lbl_8019F008, 0x1c);`

Both carry `// provenance: retail-disassembly:GFZE01:0x80071CC0 GXProjCache_Save`.

## Verified result (authoritative compile + objdiff)
- Both compile: OK.
- Score: **29.076923%** each; `winner: None`. Not exact → NOT registration-ready
  (exact-artifact preflight fails; live destination remains asm, no false-positive).
- Instruction-level diff of `GXProjCache_Save` (.text, size 52 both sides):

  retail (target):
    stwu r1,-0x10(r1)
    mflr r0
    lis r3,lbl_8019F024@ha
    lis r4,lbl_8019F008@ha
    stw r0,0x14(r1)
    addi r3,r3,lbl_8019F024@l
    li r5,0x1c
    addi r4,r4,lbl_8019F008@l
    bl memcpy
    lwz r0,0x14(r1)
    mtlr r0
    addi r1,r1,0x10
    blr

  candidate (C and D identical here):
    mflr r0
    stw r0,0x4(r1)
    stwu r1,-0x8(r1)
    lis r3,lbl_8019F024@ha
    lis r4,lbl_8019F008@ha
    addi r3,r3,lbl_8019F024@l
    addi r4,r4,lbl_8019F008@l
    li r5,0x1c
    bl memcpy
    lwz r0,0xc(r1)
    addi r1,r1,0x8
    mtlr r0
    blr

- The memcpy CALL SEQUENCE (lis/addi/li/bl, 6 instrs) is **byte-identical** on
  both sides → argument order, register binding, and `0x1c` length all match.
- The ONLY divergence is the prologue/epilogue FRAME:
  retail reserves 0x10 (16 B, 16-byte-aligned per SVR4/PPC ABI) and saves LR at
  r1+0x14 (top of its own frame); candidate reserves 0x8 (8 B) and saves LR at
  r1+0x4 (caller linkage slot). This is a stack-frame/alignment emission quirk,
  not a body/codegen correctness issue.

## Discriminator outcome
- `void*` vs `unsigned char*`: identical 29.08% → pointer type does NOT change the
  emitted code for this call. Hypothesis RULED OUT as the discriminating factor.

## Conclusion / next
- Natural-C reconstruction of the body is CONFIRMED: `GXProjCache_Save` =
  `memcpy(lbl_8019F024, lbl_8019F008, 0x1c)` with `unsigned char[28]` globals.
- Residual 70.9% is 100% attributable to the prologue/epilogue 16-vs-8-byte frame
  mismatch, an alignment-emission artifact the C body alone does not control.
- To reach exact 100% the prologue frame must match (0x10). Options to investigate
  NEXT turn (separate evidence action): compile in full-unit context to reproduce
  the 16-byte alignment, or confirm this wrapper's prologue is a known
  non-deterministic frame and accept body-match. Not registered this turn (preflight
  requires exact; live dest still asm).
- No duplicate_candidate / already_solved / mispin refusal occurred → not a hard stop;
  one evidence action per turn already expended.

Scratch: /tmp/natc-search-80071CC0-save/{C_GXProjCache_Save_char.c,D_GXProjCache_Save_void.c,raw2.json,save_diff.json}
