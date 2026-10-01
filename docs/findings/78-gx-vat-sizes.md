# Finding 78 — GX VAT-size calculator

Symbolic-only rename on `contrib-20260825-gx-vat-sizes`.

- `fn_80032BE0` -> `__GXCalculateVatSizes`
- The helper sums per-attribute vertex strides using the GX size lookup tables,
  writes the resulting vertex-size state into `__GXData`, and is reached from
  the VAT dirty-state path. This matches the internal GX SDK VAT-size
  calculator identified by the fresh GX audit.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
