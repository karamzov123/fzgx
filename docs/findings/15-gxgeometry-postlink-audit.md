# 15: post-link word-audit is the ground truth for sdata2-reloc units; GXGeometry carve

Date: 2026-08-22. w4-gxgeom agent. Unit coarse/text_8003458C.c →
dolphin/gx/GXGeometry.c partial (0x8003458C–0x8003493C, 944B, 9 funcs).

## Result

__GXSetDirtyState (natural C) + GXBegin/__GXSendFlushPrim/GXSetLineWidth/
GXSetPointSize/fn_80034834/GXSetCullMode/fn_800348DC/__GXSetGenMode
(nofralloc asm bodies). All EXACT in the LINKED dol; gate sha1 green.
Leftover: coarse/text_8003493C.c (0x8003493C–0x800372E0).
Renames: fn_8003458C→__GXSetDirtyState, fn_8003462C→GXBegin,
fn_8003471C→__GXSendFlushPrim, fn_800347A4→GXSetLineWidth,
fn_800347EC→GXSetPointSize, fn_80034890→GXSetCullMode,
fn_80034918→__GXSetGenMode. Scope flips to global: those + fn_80032B8C/
fn_80032BE0/fn_80033650/fn_8003666C/fn_80036F24/fn_800348DC/fn_80034834.

## Finding 1: unit_check.py pre-link compare is USELESS for r2/r13-pointer units

`extern GXData *const gx;` compiles to `lis/addi + lwz 0(rX)` placeholder +
type-109 (SDA2-style) relocs pre-link; retail shows `lwz rD,-0x7de8(r2)`.
unit_check.py only masks branch slots → every gx access scores as a fake
diff. GXMisc.o has the SAME placeholder form pre-link yet links exact —
the linker rewrites the instruction (rA→r2, disp filled). CORRECT LOOP:
register unit → ninja main.dol → slice the LINKED dol at each function's
vaddr vs orig (relocs resolved; any diff is real). Script pattern:
compare linked-vs-orig per function (cmp_geo.py style), not .o-vs-dol.

## Finding 2: gx pointer hoisting variants (all three wrong for GXBegin)

- `extern GXData *const gx;`: MWCC hoists the pointer load across the
  opaque bl calls (keeps r6, uses addi offsets) ≠ retail reload-per-test.
- `*volatile const`: reloads pointer before EVERY access incl. the first
  (retail shares one load across check+first test) ≠ retail.
- plain `*gx`: still hoisted.
Retail shape (load once for outer `!=0` check + first bit test, reload
after each call) was NOT reachable from natural C at GC/1.2.5n → asm body
transcription used instead. __GXSetDirtyState (no outer check) matched
naturally on first try apart from ONE mask bug.

## Finding 3: dirtyState test masks decoded (retail truth)

Six tests in order: &1→fn_8003666C, &2→fn_80036F24, &4→__GXSetGenMode,
&8→fn_80032B8C, &0x10→fn_80033650, **&0x18**→fn_80032BE0. The last is
bits 3|4 combined (NOT 0x30); bit 4 is tested twice by design.

## Traps

- Worktree provisioning stages the orig SYMLINK as a new file (`A orig`);
  MUST `git rm --cached orig` before committing or cherry-pick clobbers
  main's baserom (findings/13 replay avoided).
- Wrong function sizes shift ALL downstream link layout: a net +0x24 size
  error showed up as ±0x24 bl-displacement diffs in an otherwise-exact
  earlier function. Read bl-displacement deltas as total-size-delta signals
  of functions BETWEEN caller and callee.
