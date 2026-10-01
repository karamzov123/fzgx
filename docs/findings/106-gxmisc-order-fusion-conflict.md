# 106: GXMisc registration blocked by asm/C interleaving vs peephole fusion

Date: 2026-08-25 (session v11).

GXMisc.c reaches 100% objdiff parity pre-link after bare-symbol sda21 edits
(gx(r2), __memReg(r13), DrawDone(r13), FinishQueue bare li). Registration in
configure.py compiles and links per-unit but the DOL gate goes RED: our object
emits functions in source order, retail needs

  SetMisc Flush ResetWG | AbortFrame(asm) SetDrawSync(asm) SetDrawDone(asm)
  DrawDone(asm) | Poke* Peek*(C) SetDrawSyncCallback TokenHandler(static)
  SetDrawDoneCallback FinishHandler(static) __GXPEInit

Moving the four asm bodies to positions 4-7 poisons clrlslwi fusion for every
following natural-C function (finding 10 reconfirmed): GXPokeAlphaMode
20->28 B, GXPokeBlendMode 136->148, GXPeekARGB 36->44, etc. Keeping asm last
(as today) leaves the linked layout wrong for the whole middle of the unit.

Escape hatches, in preference order:
1. Natural-C all four asm funcs so no `asm` keyword precedes the C block.
   Melee's GXAbortFrame calls __GXCleanGPFifo; F-Zero inlined the two
   memReg[0x4e/0x50] poll loops plus three OSGetTime delay loops - a faithful
   C transcription is nontrivial but bounded. GXSetDrawSync pilot reached
   45/46 instrs before timebox.
2. objdiff/dtk alias-symbol support for interior labels would unlock other
   units first (see text_8006E1B0 note).
3. Accept reloc-tolerance: the DOL is already byte-exact via the coarse split;
   registration is only worth it for matched_code optics.

sda21_fix.py bugs found this session: 'extern extern' doubling, invalid
'sym+0x4[4]' externs, stray injected comment at insertion point, no filtering
of scope:local/coarse-only targets. Patch before next mechanical wave.
