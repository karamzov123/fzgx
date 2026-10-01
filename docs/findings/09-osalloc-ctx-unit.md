# 09: OSAllocCtx.c landed (allocator context swap); prologue trap reconfirmed

Date: 2026-08-22. Session 7 (w2 worktree, region 0x800055E0-0x80009FA4).

## Result

dolphin/os/OSAllocCtx.c @0x80009468-0x8000961C (3 funcs, 0x214B):
- fn_80009468 (0xB4): init of 8 allocator-context slots at lbl_8015BE40
  {-1,0,0,0,0}x8 + lbl_801A6730=-1. nofralloc asm body.
- fn_8000951C (0x88) / fn_800095A4 (0x78): swap-in/swap-out of allocator
  state {__OSCurrHeap=lbl_801A6410, HeapArray=lbl_801A6744,
  NumHeaps=lbl_801A6740, ArenaStart=lbl_801A673C, ArenaEnd=lbl_801A6738}
  against slot[lbl_801A6730] / temp lbl_8015BEE0. nofralloc asm bodies.
Gate: main.dol sha1 421c88106697d3275a3fc26fb7a01bf6d816b271 GREEN,
SDK 18/18 files 100% linked. Unit complete:true; fuzzy 86.70% purely from
hardcoded-literal relocs (lis 0x8016/-0x41C0/-0x4120 + r13 disps).

## Region identification (for successors)

0x80008E84-0x80009FA4 is the GX allocator family = melee OSAlloc.c PLUS a
GX/AV extension: 8 context slots letting code switch allocator arenas.
Mapping: OSSetCurrentHeap_thunk@8E84 (=OSSetCurrentHeap w/ swap),
fn_80008EC8=OSInitAlloc(+call to fn_80009468), fn_80008F60/F88=OSCreateHeap
wrappers setting flag lbl_801A6734=0/1 then fn_80008FB0=OSCreateHeap body,
fn_80009064=OSDestroyHeap w/ swap, fn_800090A4=OSCheckHeap (assert lines
0x350-0x37C vs melee's 0x37D+; strings base lbl_801221C0), fn_8000961C=
create-heap-in-slot?, fn_80009830=alloc-from-range?, fn_80009AA8=
free-to-range?, fn_80009CAC wrapper -> fn_80009CD4=carve allocation out of
free range, returns NEGATIVE leftover or 0. HeapDesc {s32 size; Cell* free;
Cell* allocated}=12B; Cell{prev,next,size}=12B. Everything else in the
coarse range is game boot code (main, DVD-status poll w/ callbacks
lbl_801A66DC/E0/E4/E8, reset-combo watcher fn_8000691C, MMU/page-table
family fn_800071B8-fn_80007A00, GX init fns calling fn_8007xxxx engine).

## Trap: build/GFZE01/asm/**.s are EXPECTED listings, not your output

dtk generates per-unit reference listings there; they always match retail.
Don't celebrate reading them - check report.json/dol bytes instead.

## Trap reconfirmed: GC/1.2.5n mflr-first prologues

Natural C for OSSetCurrentHeap_thunk/fn_80008EC8 etc. under the DolphinLib
pin produced `mflr r0; stw r0,4(r1); stwu r1,-X(r1)` B-prologues with
different frame sizes vs retail A-pattern `stwu r1,-X(r1); mflr r0;
stw r0,X+4(r1)`. All framed functions mismatch => converted to nofralloc
asm bodies. WIP natural-C attempt preserved nowhere (deleted); rewrite from
this finding if pattern-A flags ever found.

## Tooling traps this session

- Inline asm REJECTS sym@ha/@l as immediates AND as load/store displacements
  ("illegal use of label"); only plain numbers or bl targets work. Hardcode:
  lbl_8015BE40 -> lis 0x8016 / disp 0xBE40(-0x41C0); lbl_8015BEE0 ->
  disp 0xBEE0(-0x4120). SDA base r13=0x801AE3C0 confirmed via finding 06.
- Branch labels inside asm bodies must NOT start with '.' ("unknown
  assembler instruction mnemonic" on `beq .L_xxx`); use bare identifiers.
- Dead-strip strikes again: unreferenced GLOBAL C functions vanish even
  when their bytes are needed (fn_80008F88/fn_80009064 case). force_active.

## Next steps

1. Re-add remaining allocator funcs as nofralloc asm bodies (transcriptions
   straightforward; retail listing = coarse/text_800055E0.s lines ~4100-5352).
2. Chase A-prologue flags to rescue natural C (finding 05 open question).
