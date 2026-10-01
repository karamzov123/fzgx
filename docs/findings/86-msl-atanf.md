# Finding 86 — MSL `atanf`

Symbolic-only math wave on `contrib-20260825-msl-atanf`.

- `fn_8006D2AC` -> `atanf`
- The function is the single-argument, table-driven arctangent companion to
  the already named `atan2f` at `0x8006D24C`, with the same MSL math-cluster
  polynomial structure and fixed-point angle conversion support.
- Its caller topology identifies it as the one-argument form used by the
  adjacent acos/rotation helper family.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
