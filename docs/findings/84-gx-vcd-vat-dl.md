# Finding 84 — GX VCD/VAT, texgen, and display-list APIs

Batched GX API correction/rename wave on `contrib-20260825-gx-vcd-vat-dl`.

Applied and corrected five symbols:

- `0x80032B8C` — `__GXSetVCD` (corrected from the prior mistaken
  `__GXSetVAT` label): writes CP VCD registers `0x50/0x60` and refreshes XF
  vertex specs.
- `fn_80033650` — `__GXSetVAT`: iterates dirty VAT entries and writes CP
  registers `0x70/0x80/0x90`, then clears the dirty mask.
- `fn_80033D4C` — `GXSetNumTexGens`: updates genMode, writes XF register
  `0x3F`, and marks GX state dirty.
- `fn_8003887C` — `GXBeginDisplayList`.
- `fn_80038944` — `GXEndDisplayList`.

The VCD/VAT correction is directly established by the register sequences in
`GXAttr.c`; the texgen and display-list identities match the corresponding GX
SDK bodies and cached scout evidence.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
