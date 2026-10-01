# 10: GXMisc matched; asm bodies poison peephole for the rest of the TU

Date: 2026-08-22. Session following GXFifo integration. Unit
coarse/text_80032370.c (0x80032370-0x800372E0) identified as GX SDK Feb 2003:
GXAttr.c + GXGeometry/GXMisc/GXFrameBuf/GXLight/GXTexture grab-bag. GXInit is
NOT here - it sits at 0x800309FC in coarse/text_8002F140.c (with __GXInitGX).

## Result

dolphin/gx/GXMisc.c: 22 funcs @0x80033D8C-0x8003458C (0x800B), linked-exact,
gate sha1 421c8810...b271. Natural C (18): SetMisc, Flush, ResetWriteGatherPipe,
Poke{AlphaMode,AlphaRead,AlphaUpdate,BlendMode,ColorUpdate,DstAlpha,Dither,
ZMode}, Peek{ARGB,Z}, SetDrawSyncCallback, TokenIntHandler, SetDrawDoneCallback,
FinishIntHandler, __GXPEInit. Asm nofralloc bodies (4): AbortFrame, SetDrawSync,
SetDrawDone, DrawDone. Symbol seeds found: fn_800110A8=OSSleepThread,
fn_80011194=OSWakeupThread (renamed in symbols.txt).

## Finding 1: an `asm` function disables clrlslwi fusion for ALL later C in the TU

With any `asm void f(void){nofralloc...}` BEFORE a natural function, MWCC
1.2.5n stops fusing `clrlwi+slwi` into `clrlslwi` (GXPeekARGB regressed 36B
EXACT -> 11-word DIFF). Moving all asm blocks to the END of the file restores
fusion. RULE: natural C first, asm bodies last (still inside the same
`#pragma force_active` push/pop region). Also: `#pragma dont_inline on`
wrapped around a global function made it VANISH from the object entirely -
do not use it to suppress inlining per-call-site.

## Finding 2: F-Zero __GXData layout (differs from Melee's)

gx = .sdata2:0x801A7058 (r2-based access! sdata2 uses r2, sbss/sdata use r13).
Offsets confirmed by disasm: unk u16@0x0, bpSent u16@0x2, vNum u16@0x4,
vLim@0x6, vatA[8]@0x1C, matIdxA/B@0x80/0x84, indexBase@0x88, indexStride@0x98,
suTs0[8]@0xB8, genMode@0x204, dlSaveContext-like byte@0x4F1, second byte@0x4F2,
dirtyVAT byte@0x4F3, dirtyState u32@0x4F4. Struct must be volatile-qualified
(retail reloads fields after stores, e.g. sth vNum; lhz vNum in GXSetMisc).
r13 base = 0x8019E3C0 (from __peReg 0x801A6BD0 @ disp 0x8810); r2 base =
0x800FEE40 (gx disp 0x8218) - hardcode displacements in asm bodies like
EXIBios.c does.

## Finding 3: switch tree shape depends on case-set cardinality

Retail GXSetMisc pivots cmpwi 2 first with a >=4 range guard: that requires FOUR
cases {0(empty),1,2,3}. Writing only {1,2,3} produces cmpwi 1 on the low side.
Adding the explicit empty `case GX_MT_NULL: break;` reproduces retail exactly.

## Finding 4: small-data relocation eligibility

extern scalar (u64 FinishQueue) gets sda21 relocs; extern ARRAY (u8[12]) does
not (lis+addi instead). Declare extern data as scalars when only its address is
taken and you need r13 addressing to match retail.

## Traps

- Build pipeline renames object symbols per symbols.txt AFTER compile: a C
  function named GXFlush links as fn_80033E20 unless symbols.txt is renamed
  too ("undefined: 'GXFlush'" from GXFifo.o was the symptom).
- PPCMtwpar takes the PHYSICAL fifo address 0x0C008000 (lis 0x0c01), not the
  uncached alias.
- GXAbortFrame structure (all inlined): gated on gx->[0x4F2] != 0 && GPFifo;
  poll memReg[39]/[40] until stable twice with OSGetTime delay threshold 8;
  then piReg[6]=1, delay thr 0x32, piReg[6]=0, delay thr 5.
