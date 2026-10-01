# 10: W3 DVD range carved; dvdlow.c landed (asm transcription + 1 natural C); gate green

Date: 2026-08-22. Session 7 (w3-dvd worktree). Range owned:
`.text 0x80015F80..0x8001A78C` (DVD library, 91 funcs, 0x480C bytes).
Baseline on entry: green at 3f576b4.

## Result

Gate restored and GREEN: `main.dol` sha1
`421c88106697d3275a3fc26fb7a01bf6d816b271`, 1414848 B, with
`coarse/text_80015F80.c` replaced by six units tiling the range exactly:

- `src/dolphin/dvd/dvdlow.c` [15F80,16DC0) — 20 funcs: __DVDInitWA NATURAL C;
  other 19 nofralloc exact-asm transcription.
- `src/dolphin/dvd/dvdfs.c` [16DC0,1776C) — 14 funcs, asm scaffold.
- `src/dolphin/dvd/dvd.c` [1776C,19E64) — 45 funcs, asm scaffold.
- `src/dolphin/dvd/dvdqueue.c` [19E64,1A05C) — 5 funcs, asm scaffold.
- `src/dolphin/dvd/dvderror.c` [1A05C,1A31C) — 4 funcs, asm scaffold.
- `src/dolphin/dvd/fstload.c` [1A31C,1A78C) — 3 funcs, asm scaffold.
- remainder renamed `coarse/text_8001A78C.s` [1A78C,1E480) untouched.

report.json fuzzy: dvdqueue 100%, dvderror 99.66%, dvdfs 97.0%, dvd 93.8%,
dvdlow 94.9%, fstload 91.1%. Fuzzy <100 on asm units = missing relocs
(hardcoded r13 displacements + @ha/@l DO carry relocs; only sda21 slots are
hardcoded). Linked bytes are exact — sha gate proves it.

## Config diffs

configure.py: 6x `Object(Matching, "dolphin/dvd/<name>.c")` appended inside
DolphinLib("dolphin", ...) after PSMathFns.c (keeps GC/1.2.5n pin).
splits.txt: coarse/text_80015F80.c entry replaced by the 6 entries above +
`coarse/text_8001A78C.c: .text start:0x8001A78C end:0x8001E480`.

## symbols.txt changes (all verified against regenerated listings)

- scope local→global flips (34): CommandList, AlarmForTimeout, BB2,
  DummyCommandBlock, WaitingQueue, bb2Buf, StopAtNextInt, Callback,
  LastResetEnd, ResetOccurred, WaitingCoverClose, WorkAroundType,
  WorkAroundSeekLocation, NextCommandNumber, BootInfo(801A68A0), FstStart,
  FstStringStart, MaxEntryNum, executing, IDShouldBe, bootInfo, PauseFlag,
  PausingFlag, FatalErrorFlag, CurrCommand, ResumeFromHere, CancelLastError,
  ResetRequired, FirstTimeInBootrom, DVDInitialized, bb2, idTmp, CurrTvMode,
  autoInvalidation, stateBusy(function).
- renames: `block$16`→blockBuf ($ illegal in MWCC identifiers);
  OS-level BootInfo@801A6748→OSBootInfo (collides with DVD BootInfo once
  global); dtk string objects `@18/@35/@36/@40/@41` → str_80XXXXXX (NOTE:
  renaming to lbl_ADDR gets REVERTED by `dtk dol split` write-back — its
  auto-label namespace collides; str_/dat_ prefixes stick).

## Findings

1. **dtk suffixes LOCAL symbols with _ADDRESS in listings/objects**
   (`stateBusy`→`stateBusy_800189FC`). Any symbol referenced cross-object
   must be flipped global or the emitted label won't match your extern.
   Function names are not suffixed; data objects are.
2. **MWCC inline asm**: function-level only (`asm void f(void){nofralloc…}`);
   inside plain functions nofralloc is rejected. `@ha/@l` work and emit real
   R_PPC_ADDR16_HA/LO. `@sda21` rejected (confirms findings/06) → hardcode
   `disp(r13)`; SDA base = 0x801AE3C0 (.sdata start 0x801A63C0 + 0x8000).
   dtk prints sda21-reloc'd `addi rX,r13,dsp` as `li rX,sym@sda21` — when
   transcribing, li+bare-sda21 must become `addi rX,r13,dsp`.
   `crclr crXcc` must be rewritten `crxor N,N,N` (cr1eq=6).
   bl targets need a C declaration even when defined later in same file.
3. **Emission order == source order** across asm/C mix in one TU. Placing a
   static C function before its retail position shifts everything after.
4. **Position affects codegen**: AlarmHandlerForTimeout compiled near-exact
   as first function in TU but produced different scheduling (no li hoist,
   mr instead of addi) once moved between asm functions. Naturalizing these
   requires matching neighbors or pragma tricks; deferred.
5. unit_check pre-link DIFF counts equal to the function's @ha/@l/sda21
   word count = false alarm (relocs read as 0); judge by linked DOL diff.

## Next steps (handoff)

- Naturalize remaining dvdlow funcs (esp. small DI wrappers; try `_DI[i]`
  macro forms vs local base pointer), then dvdqueue/dvderror (already ~100%
  fuzzy), then dvdfs/dvd/fstload. Switch jumptable functions (stateBusy,
  fn_80018D1C, fn_800198FC, fn_80019C50, ErrorCode2Num region) carefully:
  natural switch() would EMIT .data tables → layout break; keep asm or carve
  their .data ranges out of coarse/data_80121EC0.c into the unit's splits
  entry before attempting C.
- Transcriber script preserved at /tmp/opencode/dvdwork/gen_asm.py
  (reads build/GFZE01/asm/dolphin/dvd/*.s, writes scaffold C).
