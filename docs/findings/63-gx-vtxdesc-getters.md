# Finding 63 — GX vertex descriptor getters

Symbolic-only rename wave on `contrib-20260825-gx-vtxdesc`.

- `fn_80032D04` -> `GXGetVtxDesc`
  - Reads the corresponding VCD field from `__GXData` through the full
    attribute switch table and writes the descriptor type to the caller.
- `fn_80032EB8` -> `GXGetVtxDescv`
  - Iterates all vertex attributes, writes each attribute/type pair using
    `GXGetVtxDesc`, and terminates the output list with `GX_VA_NULL`.

These bodies match the Melee Dolphin `GXAttr.c` implementations and are
used by the GX vertex-attribute subsystem.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
