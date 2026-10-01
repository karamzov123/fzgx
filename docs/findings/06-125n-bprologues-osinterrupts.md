# 06: GC/1.2.5n produces B-prologues; OSInterrupts extended; objdiff reloc penalty

Date: 2026-08-21. Follows findings/05. Partially supersedes findings/05's
"OPEN QUESTION": the B-pattern trigger is (at least partly) the compiler
VERSION, not a flag on GC/1.3.

## Unit result

dolphin/os/OSInterrupts.c extended to 0x8000D4F4–0x8000D5E4 (6 funcs):
- OSDisableInterrupts/OSEnableInterrupts/OSRestoreInterrupts: asm blocks,
  EXACT (unchanged).
- __OSSetInterruptHandler@0x8000D540 + __OSGetInterruptHandler@0x8000D55C
  (renamed from fn_8000D55C in symbols.txt): NATURAL C, 100% under GC/1.2.5n.
  Index param must be `short` — that is what produces retail's `extsh r0,r3`.
  MWCC emits `sym@sda21(r0)`-form relocs (type 109) for the extern pointer;
  mwldeppc fixes them up correctly. GC/1.3 gives WRONG selection for these
  (fused rlwinm+lwzx/stwx, no extsh) → first natural-C validation of the
  DolphinLib mw_version pin GC/1.2.5n.
- __OSInterruptInit@0x8000D570: nofralloc-asm fallback body, linked bytes
  EXACT, sha1 gate green.

Gate after change: ninja OK, main.dol sha1 421c8810...b271, 1414848 bytes.

## Finding 1: B-prologues come from GC/1.2.5n

findings/05 asked which flag/version yields prologue
`mflr r0; stw r0,X(r1); stwu r1,-Y(r1)` (retail: 540 funcs). Answer:
GC/1.2.5n emits mflr-FIRST prologues by default (GC/1.3 always stwu-first).
With `-schedule off` (or -O<=3), the 1.2.5n prologue of __OSInterruptInit is
byte-exact through instruction 6. `-O4,p` implies `-schedule on` and hoists
independent work (`li` args) into the prologue gap → non-retail shape.

## Finding 2: pure-C __OSInterruptInit still doesn't byte-match

Under GC/1.2.5n (-O2,p/-O3,p/-O4,p -schedule off all tried, melee-shaped
source incl. OSPhysicalToCached macro, __PIRegs as extern array vs literal
base): prologue exact but ~6 genuine body diffs remain:
- memset arg evaluation order: ours lwz(table),li,li; retail li,li,lwz(table)
- PI base formation/store interleaving vs the 0xC8 store
- handler address @ha/@l split: ours lis r4/addi r4; retail lis r3/addi r4,r3
Retail init has NO relocs for the PI access (literal 0xCC003004) and NO
hoisting → consistent with 1.2.5n + schedule off, but exact scheduling not
reproduced. Kept asm body. If someone wants pure C later: start from
/tmp/opencode/ostint/fullB.c and chase the three deltas above.

## Finding 3: objdiff penalizes missing relocations even when linked bytes match

The asm-body init hardcodes `-0x7C18(r13)` (SDA displacement of
InterruptHandlerTable; SDA base = .sdata start + 0x8000 = 0x801AE3C0) and
`lis r3,0x8001; addi r4,r3,-0x22F0` (= @ha/@l of ExternalInterruptHandler
0x8000DD10). Linked DOL is byte-exact, but the object lacks retail's 4 relocs
(2× sda21 + ADDR16_HA/LO) so objdiff fuzzy = 95.34% for that function while
the other 5 score 100%. Consequence: prefer natural-C (or reloc-equivalent
forms) when possible; hardcoded-literal asm bodies cap below 100% fuzzy.

## Tooling notes

- unit_check.py defaults to --version GC/1.3; DolphinLib units MUST be
  checked with `--version GC/1.2.5n` (configure.py pins that for dolphin).
  Wasted cycles on 1.3 before noticing.
- unit_check word classifier treats ANY 48/4B-prefixed word as "BL" on both
  sides → unrelocated `bl` placeholders (48000001) read as BLTOLERANT even in
  EXACT verdicts. Fine pre-link; sha gate is the real judge.
- mwcceppc inline asm REJECTS `sym@sda21` ("end of line expected"). Symbol
  suffix relocs are only reachable via natural C data access. (@ha/@l untested
  inline — sda21 failure aborted first.)
- After editing symbols.txt/splits.txt: run configure.py, delete
  build/GFZE01/config.json (+ stale auto_* obj/asm) to force re-split, then
  ninja. The split depfile does NOT track those files.

## Scope flips done (symbols.txt)

- InterruptHandlerTable (.sbss:0x801A67A8) local→global (cross-object ref
  from setter/getter/init).
- ExternalInterruptHandler (.text:0x8000DD10) local→global (address taken by
  init). Bytes unaffected; dtk regenerates listings with .globl.
