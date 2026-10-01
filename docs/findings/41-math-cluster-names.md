# Naming study — math/MTX-adjacent cluster
Targets: fn_8006D0B4, fn_8006D24C, fn_80072CC4, fn_80072C24, fn_80072E20, fn_80072D64, fn_800734A8

Method: disassembled each target plus helpers from the DOL via `tools/dump_asm.py`
(build/ obj dirs absent in this worktree; capstone available at `.venv/bin/python`).
Cross-referenced findings/32-mtx-names.md, findings/26-fn-profiles.json,
config/GFZE01/symbols.txt, and Melee baselib/MSL refs.

## Verdict summary

| VA | proposed name | class | confidence |
|---|---|---|---|
| 0x8006D0B4 | `sqrtf` (MSL) | MSL math | HIGH |
| 0x8006D24C | `atan2f` (MSL) | MSL math | HIGH |
| 0x80072C24 | `HSD_MtxLoadCached4F`-style: cached 4-word param setter (cache slot +0xF0) | game/model state | MED |
| 0x80072CC4 | same family, cache slot +0x1F0 | game/model state | MED |
| 0x80072D64 | same family, 5-word record @+0x2F0 (stride 0x14) | game/model state | MED |
| 0x80072E20 | same family, 5-word record @+0x430 (stride 0x14) | game/model state | MED |
| 0x800734A8 | same family, material/light cache @+0x570 (+0x720 table, stride 0x24) | game/model state | MED |

## Evidence

### fn_8006D0B4 = sqrtf (HIGH)
```
frsp f0,f1; lfs f2, [1.5]; mcrfs/bso NaN paths; bl 0x8006D088; fmuls f1,f1,f0; blr
0x8006D088: frsqrte f1,f1 + two fnmsubs/fmuls Newton-Raphson refinement steps
0x8006D04C: infinity-sign fixup path (li r0,0 / 0x7F80 / 0xFF80 -> lfs)
```
Classic MSL `sqrtf`: frsqrte seed + 2 NR iterations, ±inf/NaN special cases,
result × x. Callers confirm: MTXLookAt prep calls it on a distance sumsq;
`fn_8006D3D0` computes `1-x²` then `bl 0x8006D0B4` (acos-style helper).

### fn_8006D24C = atan2f (HIGH)
```
mfcr; mcrfs cr7/cr6 <- fpscr fields 3/4 (x/y classify); bso -> special-case tails
fabs both, pick larger, fdivs smaller/larger
stfs to scratch; bit-twiddle exponent (rlwinm/oris 0x80, subfic 0x87)
-> index into a float coefficient table (addis -0x7feb / addi 0x2860 base),
   fmadds poly + fctiw; sign reconstruction (subfic 0x4000, neg, ±0x8000)
returns r3 = fixed-point angle in 0x4000 == π/2 units (i.e. ±π mapped to ±0x8000)
```
Table-driven polynomial atan with quadrant/sign fixups and fp-classify prologue:
unambiguous MSL `atan2f` shape. Caller evidence: `fn_8006F394`
(proposed C_MTXRotAxis helper) multiplies vec components by a scale const and
calls it; `fn_8006D3D0` implements acos via sqrt(1−x²)/atan2f branch pair.
Companion `fn_8006D2AC` = atanf core (same table/poly body, single-arg).

Note: returns an int (r3) scaled angle, matching Melee's `lbtrigf.c` atan2f
decomp convention (`float atan2f(float,float)` at 022C30 there returns the same
fixed-point encoding through float bits). Keep the MSL name.

### fn_80072C24 / CC4 / D64 / E20 / 34A8 — NOT math library (MED)
None of these touch FPU regs or paired singles. All five share an identical
template:
1. save lr, call small trampolines 0x8007988C/0x80079888 (and epilogue pairs
   0x800798D8/0x800798D4) — these are the sda-based critical-section /
   per-context accessors documented around 0x80079xxx;
2. compute slot address from an id argument into a global array based at
   `lwz r0,-0x7688(r13)` (a model/context struct pointer):
   - C24: `(id<<4)+0xF0`, 4-word dirty-compare
   - CC4: `(id<<4)+0x1F0`, 4-word dirty-compare
   - D64: `id*0x14+0x2F0`, 5-field compare incl. one byte field
   - E20: `id*0x14+0x430`, identical shape to D64
   - 34A8: `(id<<4)+0x570`, branches on arg2<8; <8 path also compares against a
     parallel table at `base+0x720` with stride 0x24 and copies two u16s
     (+0x20/+0x22) from it; ≥8 path writes constant 0xFF sentinel word.
3. on mismatch, call the corresponding hardware-write routine
   (0x800370A0 / 0x800370E4 / 0x80037128 / 0x80037190 / 0x800375F0) and store
   the shadow copy.
0x800370A0 confirmed as a direct HW poker: builds a packed word with rlwimi of
four args into `0xCC00xxxx` (`lis r3,0xCC01; stb/stw -0x8000(r3)` = GX FIFO reg
write region) and shadows to `obj+0x130`.

So this cluster = **cached GX register setters** (shadow-copy + write-through on
change), part of the game's model/material state layer (callers: main.o,
MTXHead unit, game/lightctrl_8007264C.o, game/model_80074D88.o,
game/model_80072EDC.o). They are *GX-register adjacent*, not MTX math. Suggest
descriptive names until the model struct lands:

- fn_80072C24 → `ModelSetCachedParam_F0(id, w0,w1,w2,w3)`
- fn_80072CC4 → `ModelSetCachedParam_1F0(id, w0,w1,w2,w3)`
- fn_80072D64 → `ModelSetCachedParam_2F0(id, w0,w1,byte,id2,w4)`
- fn_80072E20 → `ModelSetCachedParam_430(...)` (same layout)
- fn_800734A8 → `ModelSetCachedMaterial_570(id, kind, val)` (dual-table,
  0xFF sentinel for kind≥8)

## Cross-check against taken symbols (symbols.txt)
No collisions: all named MTX symbols live at 0x8006E250–0x8006FB20; the two
MSL math names (`sqrtf`, `atan2f`) and the five game-side names above are free.
