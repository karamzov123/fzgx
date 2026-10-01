# findings/103 — MWCC sym+offset relocation rules (session v17, 2026-08-25)

Tested empirically with GC/1.2.5n mwcceppc + readelf on scratch objects.

## What WORKS
1. `sym+0x28@ha` / `sym+0x28@h` / `sym+0x28@l` in asm ->
   R_PPC_ADDR16_HA / _HI / _LO with addend 0x28.
   NOTE: retail sometimes shows @h (ADDR16_HI, no carry) where @ha would add a
   carry bit — check the retail reloc type per site; @h != @ha when low16 >= 0x8000.
2. Bare `sym+offset` D-form operand (`lfd f0, __OSStartTime+0x4`) ->
   R_PPC_EMB_SDA21 with addend. THIS IS THE BIG ONE: the "offset-addend sda21"
   blocker class from v11–v15 is MECHANICALLY REACHABLE after all.
   - Works for r2 (sdata2) and r13 (sdata/sbss) targets.
   - The `sym+offset@sda21` explicit-suffix form does NOT parse ("end of line
     expected") — bare form only.
3. In-body asm labels (`label:` inside an asm fn body) referenced by
   `b label` with NO extern declaration -> raw branch, NO reloc emitted.
   This defeats the REL24-reloc problem for tail branches / interior labels
   within the same function (OSInterruptMask __OSDispatchInterrupt proven).
   With an extern decl present, MWCC emits R_PPC_REL24 instead.

## What FAILS
- `sym+offset(r2)` / `sym+offset(r13)` D-form -> emits R_PPC_ADDR16 (type 3)
  against absolute address; linker rejects: "Relocation (3) ... out of range".
  Do NOT use for sda targets; use bare sym+offset (rule 2).
- `lfd/lfs fX, sym` without offset but bare (no addend needed) also gives
  ADDR16 not SDA21 — the SDA21 emission requires... actually rule 2 works;
  plain bare-name-without-offset was already known to emit SDA21 with sized
  extern <=8B. For >8B externs use this offset trick only if a genuine split
  symbol exists; otherwise keep numeric.

## Landed using these rules (v17a–f, all gate GREEN 421c8810…b271)
- v17a cf1b78a: MTX.c PSMTXMultVecSS split dead block lbl_8006E2C0 + lbl_8006E2D0
  (named cross-fn b needs in-file `asm void lbl_8006E2D0(void);` decl). Unit 100%.
- v17b cad26e7: metrotrk/init gap_01_8008CF44_text (4B blr) split out of
  InitMetroTRK tail — the v15 "layout problem" was just this trailing blr. 99.97%.
- v17c 9e7ef78: MTXFused MathSin/MathSinCos lis/ori lbl_801327F8+0x28 @h/@l.
  MathSinCos 100%. (@ha vs @h matters: retail uses @h here.)
- v17d 4b0a22d: OS.c OSInit __OSStartTime+0x4 and __OSVersion+0x4 bare-offset
  sda21 sites. OS unit 99.99%.
- v17e 51da3d1: dvdlow LastResetEnd+0x4 x2 -> dvdlow 100%.
- v17f 677800d: OSError/OSResetSW/OSInterruptMask same treatment + in-body
  labels for _8000dc68/_8000dc6c -> both units 100%.

## Remaining MTXFused residue (post v17c)
- fn_8006D3D0/fn_8006D46C lbl_801A73B4+off sites: bare form emits SDA21 with
  addend pre-link BUT objdiff still flags; (r2) form links RED (out of range).
  These five constants live at .sdata2 0x801A73B4 size 0x1C — consider renaming
  sub-symbols or natural C.
- MathFastInvSqrt `bl` sites: retail uses raw `bl .+0x90` (self-contained fn?),
  ours emits REL24 — likely same class as text_8006E1B0 interior-label issue.
