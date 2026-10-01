# 09: B738–D4F4 batch: OSCacheRest/OSContext/OSError/Yay0Decode; dtk reverts @NN renames; SDA/SDA2 bases confirmed

Date: 2026-08-22. Session on worktree `.worktrees/w2-context` (baseline green).
Converts the whole former `coarse/text_8000B738.c` range (0x8000B738–0x8000D4F4,
7612 B, 32 funcs) into four matched units. **Gate re-verified green after every
step: sha1 `421c8810...b271`, 1414848 B.** `dtk dol diff`: OK.

## Result

| unit | range | funcs | fuzzy | notes |
|---|---|---|---|---|
| OSCacheRest.c | B738–BBAC | 9 | 99.89% | LCEnable + DMAErrorHandler are natural C |
| OSContext.c   | BBAC–C41C | 13 | 99.96% | OSClearContext + __OSContextInit natural C |
| OSError.c     | C41C–CAC8 | 4 | 98.81% | all asm (va_list + jump table) |
| Yay0Decode.c  | CAC8–D4F4 | 6 | 97.30% | asm ('Yay' asset decoder, not dolphin OS) |

All fuzzy <100% is missing-reloc penalty ONLY (lis/addi literals + hardcoded
r13/r2 displacements have no reloc entries); linked bytes exact per sha1 gate.
SDK Code after integration: 21/21 files, 146/189 funcs, 16952/30320 B, 100% linked.

## Finding 1: dtk REVERTS renames of auto-detected (@NN) data symbols

Renaming `@69 = .data:0x80122828` to any legal identifier in symbols.txt is
undone by the next `dtk dol split` (config.json regeneration), which re-derives
auto names for detected string/data objects. Scope edits on the same lines DO
survive — only name changes revert. Consequence: string anchors and small-data
accessed from new units must use either (a) literal `lis rX,0x8012 / addi rY,rZ,imm`
in asm bodies (byte-identical to retail @ha/@l when low half < 0x8000), or
(b) `(char*)0xADDR` casts in C (MWCC emits lis/addi; hoists into r31 across
calls exactly like retail's pooled-string base pointer). Do NOT burn time
renaming @NN symbols.

## Finding 2: SDA/SDA2 base addresses CONFIRMED empirically

- SDA base r13 = .sdata start + 0x8000 = **0x801AE3C0** (finding 06 was right;
  my first displacement table had digit transpositions: lbl_801A6798=-0x7C28,
  679C=-0x7C24, 67A0=-0x7C20, 67A4=-0x7C1C, 6430=-0x7F90, 6434=-0x7F8C,
  6438=-0x7F88, __OSLastInterrupt=-0x7C10, Srr0=-0x7C14, Time=-0x7C08/-0x7C04).
- SDA2 base r2 = .sdata2 start + 0x8000 = **0x801AEE40**
  (lbl_801A6F40=-0x7F00(r2), lbl_801A6F44=-0x7EFC(r2)).
- Retail OSSetErrorHandler/__OSUnhandledException/Yay0 funcs read these via
  sda21 relocs that mwldeppc rewrites at link (findings/07) — but inline asm
  cannot express sym@sda21, so hardcode disp(r13)/disp(r2). Linked-exact.

## Finding 3: crclr cr1eq rejected; vararg-call marker in asm

mwcceppc GC/1.2.5n rejects `crclr cr1eq` ("illegal forward label ... cr1eq").
Use numeric CR-op form **`crxor 6,6,6`** (cr1eq = CR bit 6). Needed before every
bl DBPrintf/OSReport in transcribed bodies to match retail.

## Finding 4: natural-C wins and walls

Wins (worth trying first for call-heavy SDK funcs):
- DMAErrorHandler: variadic signature + 9 OSReports + mask tests all matched as
  C with `char* strBase = (char*)0x80122828;` FIRST statement → compiler
  materializes anchor into r31 before PPCMfhid2(), offsets via addi — retail shape.
- fn_8000B804 LCEnable wrapper, __OSContextInit (`*(u32*)0x800000D8=0` +
  DBPrintf literal-anchor), OSClearContext (`context->mode/state=0` +
  compare against *(OSContext**)0x800000D8). OSClearContext being real C lets
  MWCC INLINE it into OSDumpContext exactly like retail (two sth + conditional
  FPU-ptr clear, no bl).

Walls:
- OSDumpContext natural C reached size+95% but scheduler put the
  current-context load directly into r30 while retail does `lwz r0,d4(r6)` +
  delayed `mr r30,r0`. Tried: direct read, volatile read, extern tiny-function
  call (NOT inlined — worse). Transcribed to exact asm instead (~190 lines).
- Anything with bare `sync`/`isync` barriers between calls (L2GlobalInvalidate,
  __OSCacheInit): no inline-asm statement form verified; asm-body transcription
  is the cheap deterministic route.
- va_list functions (OSReport/OSPanic): retail builds a 3-word va_list with a
  constant 0x100 first field; not worth reverse-engineering stdarg shapes — asm.

## Traps (each cost cycles)

- After editing splits/symbols: full reset dance required
  (`rm -f build/GFZE01/config.json && rm -rf build/GFZE01/{asm,obj}` +
  configure.py + ninja) — findings/06 rule, reconfirmed.
- Function symbol NAMES must match symbols.txt exactly if ANY coarse object
  references them (linker error "undefined: 'fn_8000B864'" etc.). I renamed
  LCLoadBlocks/LCStoreBlocks/LCQueueWait/__LCEnable in C and got undefined refs
  from text_80071CC0.o — keep auto fn_XXXXXX names unless you also update every
  referencing coarse object (they regenerate from symbols.txt, so renaming
  requires a symbols.txt rename + full split regen).
- `NULL` is unavailable (no headers) — use 0.
- u8/u16 typedefs must exist per-file; MWCC stops at first error (-maxerrors 1),
  so fix one file at a time top-down.
- My OSDumpContext asm initially mis-chained the loop-skip branches: retail uses
  THREE chained `b +4` then body, with the loop-bottom `blt` targeting BODY
  (not the chain head). Backchain walk differs: third b jumps +0x20 over the
  body to the check. Transcribe branch OFFSETS, not just targets.
- difflib word-diff of .o vs DOL must add section file offset (o=0x34) to
  function offset, and mask bl/bc displacements before comparing.

## Unblocks enabled downstream

- OSDumpContext, OSReport, OSPanic now defined globally — DBInterface.c-style
  debug paths and future exception units can call them naturally.
- fn_80079764/fn_8000FCD4 flipped global (used cross-object by this batch).
- Remaining known sub-100% fuzzy in these units is reloc-parity only; anyone
  chasing 100% objdiff should convert the hardcoded literals back to
  sym@ha/@l once a way to persist @NN renames exists (e.g. upstream dtk fix).
