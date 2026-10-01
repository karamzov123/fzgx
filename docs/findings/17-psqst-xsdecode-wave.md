# 17: capstone misdecodes psq_st as modern-VSX names; batch 4-5 waves complete the GX core

Date: 2026-08-22. w4-gxgeom agent. Batches 4-5:
- GXGeometry.c extended +14 funcs @0x800355B0-0x80036070 (2736B)
- NEW dolphin/gx/GXTexture.c: 31 funcs @0x80036070-0x800372E0 (4720B)
All EXACT in linked dol; gate sha1 green; main at 22140B/192 funcs/37 files.
GX SDK core region 0x800307CC-0x800372E0 now fully carved except
coarse/text_80032370.c head (GXAttr block, jump-table fns pending).

## Finding 1: capstone decodes Gekko psq_st as POWER7 "xsaddsp"

Opcode 60 IS `psq_st` (56=psq_l, 57=psq_lu, 60=psq_st, 61=psq_stu). Capstone
SUCCESSFULLY decodes these words but prints modern VSX mnemonics
(`xsaddsp f5, f4, f0`), so "undecodable word" fallbacks never fire. Post-
process dump lines whose mnemonic starts with xs/xv: re-decode manually as
D-form ps ops: frS=(w>>21)&31, rA=(w>>16)&31, W=(w>>15)&1, i=(w>>12)&7,
d=(w&0xFFF sign-extended 12-bit). Same class likely for other opcode-60
words. mwcceppc accepts `psq_st f5, 0(r4), 0, 0` syntax directly.

## Finding 2: raw-word directives do NOT work in CW inline asm

`.long`/`.4byte`/`dc.l` inside `asm{}` all rejected by mwcceppc. Every byte
must come from a valid instruction mnemonic — no escape hatch.

## Finding 3: true jumptable holders transcribe verbatim

fn_80035B88 contains `mtctr r0; bctr` mid-function (real computed jump,
table via lis/addi+r9 indexed load). Transcribed verbatim → EXACT. The
generator needs NO special support; just don't treat bctr as a branch-
target-taker (BRANCH_RE excludes it correctly).

## Finding 4: forward intra-file asm bl still needs prototypes (refines 10-F2)

When a function that was previously EXTERN becomes defined later in the same
TU (e.g. fn_80035B88 absorbed into GXGeometry.c), the old `extern void
f(void);` conflicts with the new signature AND deleting it makes earlier asm
bl fail ("undefined label"). Fix: forward-declare with the FULL new
signature before first use.

## Traps

- Regex `[0-9a-f]` silently misses uppercase hex addresses (0x800360D8):
  symbol-range scans flipped only 7 of 31 functions on first pass. Use
  [0-9a-fA-F]. Symptom: dtk link errors "undefined symbol" or silent
  non-match downstream.
- Stale cherry-pick state: an accidental `git cherry-pick <old-sha>`
  leaves conflicted index; a subsequent correct pick fails confusingly.
  Always `git cherry-pick --abort` + verify clean tree before picking.

## Next in region

- coarse/text_80032370.c head (0x32370-0x33D8C): GXAttr block, 7 jump-table
  fns (SetVtxDesc/v, SetVtxAttrFmt/v, SetTexCoordGen2...) — findings/12 map;
  natural candidates __GXXfVtxSpecs/__GXSetVCD/vLim-helper/ClearVtxDesc/
  __GXSetVAT/GetVtxAttrFmtv/SetArray/InvalidateVtxCache.
- coarse/text_800372E0.c onward: next GX texture/GXFog-ish tail.
