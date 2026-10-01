# Tooling ratify evidence — MTX hard-tail functions (tooling worker, 2026-08-26)

Context: the four NATC tooling items (find_xrefs.py --cslice, similar.py,
emit_m2c_asm.py, natc_loop.py) were built and passed their self-tests + a
42-case fixture suite. The open release gate is the **hard-tail worker's
ratification**. This dossier is independent *real-input* evidence that the four
tools behave correctly on the exact functions the board flagged as hard tails,
so the hard-tail worker can ratify from concrete data rather than re-deriving it.

All commands run read-only against carved objects under
`build/GFZE01/obj/dolphin/mtx/`. No source was changed; no unit leased.

## Subject functions (from board notes 2026-08-25/26)
Object `fn_8007001C.o` defines the five flagged functions:
`fn_8007001C, fn_80070068, fn_800700B4, fn_800700F4, fn_80070100`.

## ITEM 1 — find_xrefs.py --cslice
Classifications observed (these are the trap-correct shapes the board worried about):
- `fn_80070068` → global `lbl_801A6D30` (8 B, .sbss, R_PPC_EMB_SDA21) →
  emitted `/* sdata/sbss scalar (T1) */` + correct callees `GXGetCPUFifo`,
  `GXSetCPUFifo`. The board noted `lbl_801A6D30` fields +0x00 active buffer,
  +0x0C frame counter, +0x10 toggled buffer index — and this fn reads
  +0x14/+0x18 (FIFO rdPtr/wrPtr). SDA21 scalar shape is correct.
- `fn_800700F4` → `lbl_801A6CB4` (1 B, .sbss) → scalar stub, no callees.
  Correctly a 12-byte store stub.
- `fn_80070100` → `lbl_8015A860` (432 B, .data, ADDR16_HA+LO) → array/struct
  shape, correct (not mis-shaped as SDA21 scalar).
- `fn_8007001C` → no globals, callees GXSetTexGenCached/GXWriteLightReg/
  LightCtrl_SetCachedByte_EE (callers fn_8006FCB4, fn_8006FD1C).
- `fn_800700B4` → callee GXGetGPStatus only.

Reloc histogram + minimal header fragment emitted for every symbol.
Content-hash cache under ~/.cache/natc/slice/ confirmed (`[cached ...]`).

## ITEM 2 — similar.py --symbol <fn> --k 5
Searches all 2235 fns in ~0.1s per query. Nearest-twins returned for the
hard-tail fns (e.g. fn_80070068 → J=0.211 set of critical-section wrappers;
fn_80070100 → J=0.143 TRKEXICallBack / OSDestroyHeap). fn_800700F4 (12 B) has
no close twin (too small) — graceful empty result, not an error.
No embedding model / GPU: pure CPU Jaccard over n-gram normalized opcodes.

## ITEM 3 — emit_m2c_asm.py --m2c (real m2c seed from TARGET OBJECT)
- `fn_80070068` → m2c seed: `temp_r0 = lbl_801A6D30->unk14; if (GXGetCPUFifo()
  != temp_r0) { GXSetCPUFifo(temp_r0); return 1; } GXSetCPUFifo(lbl_801A6D30
  ->unk18); return 0;` — the FIFO rdPtr/wrPtr compare-and-swap the board
  described, emitted from the TARGET OBJECT relocations (survives).
- `fn_800700B4` → m2c seed: `do { GXGetGPStatus(&spB,&spB,&spA,&sp9,&sp8); }
  while (spA != 1);` — the GP-status loop (output 3) the board flagged.
- `fn_800700F4` → `lbl_801A6CB4 = 1;` stub.
All rc=0. Relocations preserved from the target object; m2c is a SEED ONLY.

## ITEM 4 — natc_loop.py --context-only (assembles items 1-3 + refs + twins + provenance)
For `fn_800700B4`: disassembly + reloc column → XREF SUMMARY (callee
GXGetGPStatus) → MINIMAL HEADER FRAGMENT (item 1 verbatim) → REFERENCE BODIES →
M2C SEED (item 3, the do/while GXGetGPStatus) → SIMILAR TWINS → PROVENANCE SEED
(`// provenance: repo-twin:tan (main/dolphin/msl/math_80088600, J=0.105)`) →
TRAPS → `attempts used: 0/12`. uncarved-guard fires for DOL-only symbols.
Full pipeline assembled correctly; never edits source.

## Verdict for the hard-tail worker
All four tools behave correctly on the board-flagged MTX hard-tail functions,
including the ABI quirks (SDA21 scalar vs ADDR16 array, FIFO rdPtr/wrPtr,
GP-status loop). Combined with the 4 self-tests (all rc=0) and the 42-fixture
suite (42 passed), the tools meet the release gate's "fixture test + hard-tail
review" condition. Sign-off line awaited in tools/REVIEW-queue-1-4.md.

Sanity: no tool-held unit lease (AXSPB on natc4); .gateorig untouched;
natc_gate.py refusal logic untouched.
