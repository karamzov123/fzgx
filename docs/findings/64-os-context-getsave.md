# Finding 64 — OSContext current/save helpers

Symbolic-only rename wave on `contrib-20260825-os-context-getsave`.

- `fn_8000BE5C` -> `OSGetCurrentContext`
  - Returns the current-context pointer from the fixed OS global slot at
    `0x800000D4`.
- `fn_8000BE68` -> `OSSaveContext`
  - Saves GPRs, SPRs, CR, LR, MSR, and related context state into the
    supplied `OSContext`.

Both bodies match the Melee Dolphin `OSContext.c` implementations and the
GX OSContext layout. High-fan-in scheduler/SI call sites were updated by the
symbol rename tool.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
