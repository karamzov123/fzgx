# 05: Dual prologue patterns; OSTime matched

Date: 2026-08-21. Follows findings/04.

## OSTime unit (matched, commit 1828dcd)

dolphin/os/OSTime.c: OSGetTime@0x8001140C (24B), OSGetTick@0x80011424
(8B), __OSGetSystemTime@0x8001142C (100B) — all EXACT. OSGetTime/Tick are
natural asm blocks; __OSGetSystemTime is a full nofralloc-asm body (see
below why). Traps: `bl ExtSym` inside asm blocks requires a visible extern
prototype or "undefined label"; `lis r6, 0x8000` assembles correctly but
`lis r6, 0x8000@h` silently encodes lis r6,0.

## The prologue question

Retail .text has TWO function prologue orders:
- A `stwu r1,-X(r1); mflr r0; stw r0,X+4(r1)` — 912 functions (e.g. main)
- B `mflr r0; stw r0,4(r1); stwu r1,-X(r1)` — 540 functions (e.g.
  ClearArena, PPCDisableSpeculation, __OSGetSystemTime)
Both store lr at old_r1+4. Exhaustive flag grid (inline/str/lmw/common/cwd/
-O variants incl ,s /ipa/sym/lang c++) on GC/1.3 and spot checks on
1.3.2r/2.0p1/2.7/3.0a5.2 reproduce ONLY pattern A. B-trigger unknown.

Consequence: any C-compiled function whose retail form uses B will not
match via plain C under our compilers yet. Workaround in use: write those
bodies as nofralloc inline-asm blocks (byte-exact by construction). This
is what __OSGetSystemTime does.

OPEN QUESTION (non-blocking): find the flag/version producing pattern B;
would let many SDK funcs match as pure C. Test candidates: GC/2.x with
-ipa file; -sym on + specific combos; compare against melee's matched
SDK asm if its baserom ever becomes available locally.
