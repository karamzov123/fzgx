# Finding 77 — GX core API wave

Batched symbolic-only GX SDK wave on `contrib-20260825-gx-core-api`.

Applied five definitive names from the fresh GX audit:

- `fn_80032370` -> `__GXXfVtxSpecs`
- `fn_80032B8C` -> `__GXSetVAT`
- `fn_800324C8` -> `GXSetVtxDesc`
- `fn_80032F48` -> `GXClearVtxDesc`
- `fn_80033A7C` -> `GXSetTexCoordGen2`

Evidence:

- `__GXXfVtxSpecs` packs VCD color/normal/texcoord state and emits XF
  register `0x1008`.
- `__GXSetVAT` emits CP VAT registers `0x50` and `0x60`, then calls the XF
  vertex-spec writer through the GX dirty-state path.
- `GXSetVtxDesc` dispatches the 25 GX attributes and merges their 2-bit types
  into VAT state.
- `GXClearVtxDesc` resets VAT and normal-count state, restores position DIRECT,
  and marks the VAT dirty bit.
- `GXSetTexCoordGen2` matches the SDK texcoord-generator setup and post-matrix
  validation path.

All five names were unoccupied in the current symbol table. No logic changed.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
