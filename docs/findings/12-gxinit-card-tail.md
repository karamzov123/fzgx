# 10 — coarse/text_8002F140 region is CARD tail + GXInit, NOT MTX (2026-08-22)

Worktree: `.worktrees/sa-mtx2`. Region `0x8002F140–0x80031C84` (~11KB) was
hypothesized to be the Matrix library. **Wrong.** Actual contents:

| Range | Identity | Status |
|---|---|---|
| 0x8002F140–0x800307CC | **CARD library tail** (dolphin/card/*) | unmatched, kept as coarse/text_8002F140.c |
| 0x800307CC–0x800309FC | **GXInit.c statics** (`__GXDefaultTexRegionCallback` fn_800307CC, `__GXDefaultTlutRegionCallback` fn_80030848, `GXResetFunc` fn_8003086C) | MATCHED, 3 funcs / 560B exact, commits 683fb8e + 44cd7c0 |
| 0x800309FC–0x80031B50 | **GXInit + __GXInitGX** (dolphin/gx/GXInit.c per melee layout) | unmatched, coarse/text_800309FC.c |
| 0x80031B50–0x80031C84 | **`__GXCPInterruptHandler`** (belongs in GXFifo.c per melee; retail places it here; sa-gx's GXFifo.c references it as extern `fn_80031B50(int,void*)`) | unmatched |

## Evidence for CARD identification

- Pre-GX block calls `fn_8002A83C` = `__CARDGetControlBlock(chan, &ptr)` and
  `fn_8002A8F4` = `__CARDPutControlBlock(ctrl, err)` (verified by reading their
  disassembly: 2-channel check `0<=chan<2`, ctrl flag @+0x10c, lock word
  @+4 set to -1, callback slot @+0xd0 cleared).
- Neighboring unit text_80026EE0 (0x80026EE0–0x8002F140) is full of EXI*
  calls (EXISelect/EXIDma/EXIImm/EXISync/EXILock...) → CARD driver.
- Error codes match CARD_RESULT_*: -1, -4 (NOFILE), -6 (BROKEN), -7 (EXIST),
  -10 (NOSPACE), -11 (READONLY), -12 (ENCODEERR), -128 (fatal/param).
- Two control blocks of 0x110 bytes at lbl_80177960 (bss, size 0x220),
  dir entries of 0x40 indexed <0x80 via `fn_8002C4BC()` base, DCInvalidateRange/
  DCStoreRange around EXI DMA, OSGetTime()/(__OSBusClock>>2) timestamps,
  default callbacks fn_80029824/fn_80029828.
- Function map vs melee card/*.c (probable): fn_8002F140/F2F8/F428/F570 =
  CARDRdwr.c internals (__CARDRead...); F5B8/F728/F7D8/F8EC = write/erase;
  F934/F9D8/FB04/FC14 = __CARDSync/create; FC5C = dir-entry rebuild
  (fn_800300F4/F3013C = CARDDelete; F30338 = async wrapper; F30380 =
  CARDStat core; F3043C = CARDWrite/SetStatus core; F30690 = XOR checksum;
  F30754 = stat+write wrapper).

## GXData struct (verified offsets from GXInit/__GXInitGX/GXFifo disasm)

Same as melee `__gx.h __GXData_struct` through texmapId[16]@0x49C. Tail
differs from melee:

- tcsManEnab u32 @0x4DC (=0), unk4e0 u32 @0x4E0 (=0)
- perfSel u32 @0x4EC, init clears bits 27-24 (rlwinm r0,r0,0,28,23)
  — NOTE: melee uses field(4,4); F-Zero SDK differs
- inDispList u8@0x4F0=FALSE, dlSaveContext u8@0x4F1=TRUE,
  unk4f2 u8@0x4F2=TRUE (also set by GXResetFunc), byte@0x4F3=0 +
  dirtyState u32@0x4F4=0 written late in GXInit
- gxData object = .bss:0x80177BA0 size 0x4F8 (scope flipped local→global);
  FifoObj lives at gxData+0x4F8 (unnamed bss; GXInit computes it as
  `&gxData + 0x4F8`, NOT a separate symbol)

Other key globals: gx = .sdata2:0x801A7058 (`extern GXData *const gx`),
gxResetRegistered (renamed from resetFuncRegistered$63, sbss 0x801A6BEC,
flipped to global), GXResetFuncInfo .data:0x8012AD30 (flipped to global),
lbl_801A6BD8/BE0/BE4/BE8 = GXResetFunc state (sdata21 displacements -0x77E8/
-0x77E0/-0x77DC/-0x77D8 on r13; gx = -0x7DE8 on r2; __memReg=-0x77EC;
__cpReg=-0x77F4).

## GXResetFunc semantics (fn_8003086C, now asm-matched)

BOOL reset-callback registered via OSRegisterResetFunction(&GXResetFuncInfo).
!final path: spins on __memReg[0x27]/[0x28] (u16 pair) until stable, packs
(v_hi<<16)|v_lo into lbl_801A6BD8, stamps OSGetTime into BE0:BE4, sets BE8.
Subsequent: same spin, diff=now-last; `if ((u64)diff/10 != 0) return FALSE`
(the ÷10 compiles to an INLINED __div2u body — subfc/subfe/xoris/neg chain);
cur==BD8 → return TRUE else restamp+return FALSE. final path:
GXSetBreakPtCallback(0), fn_80034378(0), fn_80034444(0), 8× stw 0 to
GXFIFO 0xCC008000, PPCSync, __cpReg[1]=0 [2]=3, gx->unk4f2=TRUE, fn_80033EB0.

## Compiler findings (GC/1.2.5n, NEW)

1. **SDA21 slots are placeholders in .o** (`lwz rX, 0(r0)`-shaped) with
   R_PPC_EMB_SDAII-type relocs (type 109, odd halfword offsets). Raw-byte
   compare against DOL must mask opcode+rD only (& 0xFC1F0000) for those
   words; they resolve identically at dtk link because section layout is
   fixed. `/tmp/opencode/cmp.py` implements this (worktree-local copy lost).
2. **`fmt==a||fmt==b||fmt==c` gets range-trick optimized**
   (`addi -a; cmplwi n-a`) — retail shows three separate cmpwi/beq. Melee's
   `!=` chain reproduces branch STRUCTURE but register allocation still
   differed → both callbacks needed asm bodies.
3. **Two-arm returns never merge to single-exit** in this compiler for
   frameless functions (early blr each arm). Retail's `li r3,0; b Lexit`
   shape was unreachable from C — asm body required.
4. **`(u64)x / 10` always emits `bl __div2u`** at these flags; the retail
   INLINED div2u body could not be reproduced from any source shape
   (literal, ULL literal, volatile var, u32 divisor).
5. **dtk listing misprints `cmplw` as `cmpw`**: bytes 7C 04 00 40 decode to
   cmplw (XO=32), listing says "cmpw". Watch for this when transcribing.
6. `$` is not valid in MWCC identifiers (resetFuncRegistered$63 renamed to
   gxResetRegistered everywhere).
7. sda21 literals CAN be hardcoded in inline asm (`lwz r0, -0x7DE8(r2)`);
   `sym@sda21`, `.long`, `dc.l`, and `cmpw.` mnemonics are NOT supported.

## Remaining work in region

- GXInit (0x890): call sequence fully mapped (OSRegisterVersion(__GXVersion),
  GXSetMisc(1,0), reg pointers, __GXFifoInit, fifo attach ×3,
  gxResetRegistered-guarded OSRegisterResetFunction, __GXPEInit,
  PPCMtwpar(0xCC008000)/PPCMthid2(|0x40000000), genMode/bpMask/lpSize,
  tevc/teva/tref/texmapId/tevKsel loop, iref, suTs0/1 loop, suScis/cmode/
  zmode/peCtrl singles, cpTex field(2,23), dirtyVAT/dirtyState,
  freqBase=__OSBusClock/500 (mulhwu 0x10624DD3>>5) & freqBase/4224
  (mulhwu 0x3E0F83E1>>10), two GX_WRITE_RAS_REG, VAT loop i<8 with
  vatA&=~2|bit30 / vatB&=~1|bit31 then FIFO writes, XF regs 0x1000←0x3F &
  0x1012←1, RAS 0x58, TexCache/TlutRegion init loops (same constants as
  melee), __cpReg[3]=0, perfSel FIFO block, __GXSetTmemConfig(0),
  __GXInitGX(), return &gxData[0x4F8]).
- __GXInitGX (0x8C4): ~90 calls mapped to melee equivalents incl.
  fn_80033A7C=GXSetTexCoordGen2 (i,1,4+i,0x3c,0,0x7d), mystery loop
  fn_800339E0(i=9..24, gx, 0), fn_800332D8(i<8, &lbl_8012AC44),
  fn_800347A4/EC=SetLineWidth/SetPointSize(6,0), identity matrix built from
  lbl_801A7068(1.0f)/706C(0.0f), viewport int→float via 0x43300000 magic +
  lbl_801A7078 double bias, colors copied from sdata2 statics
  lbl_801A705C(clear)/7060(black)/7064(white), rmode switch handles
  VI_EURGB60(5)→lbl_8012B0E4 extra case vs melee.
- fn_80031B50 __GXCPInterruptHandler: fully analyzed, C draft matched
  modulo BL/SDA before trim (cpEnable fields 1,28/29/26; cpStatus 1,30/0/27;
  BreakPointCB called UNPROTOTYPED — declare `void (*BreakPointCB)()`).
- CARD tail 22 funcs: need CARDControl reconstruction across two units.
