# Finding 80 — CARD synchronous completion wave

Batched symbolic-only CARD wave on `contrib-20260825-card-sync`.

Applied the remaining direct CARD API names from the study:

- `fn_8002FC14` -> `CARDFastDelete`
- `fn_800300F4` -> `CARDSetStatus`
- `fn_80030380` -> `CARDFastOpen`

These are the synchronous/fast counterparts to the already named async CARD
entry points. The CARD study identifies their control-block, directory/status,
and fast-open body roles in the contiguous CARD API region. No logic changed.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
