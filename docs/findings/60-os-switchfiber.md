# Finding 60 — OSContext fiber switch helper

Symbolic-only rename on `contrib-20260825-os-switchfiber`.

- `fn_8000BFEC` -> `OSSwitchFiber`
- The body stores the supplied PC at context offset `0x198`, stack at offset
  `0x04`, installs the standard MSR value `0x9032`, clears the context fields,
  and branches directly to `OSClearContext`.
- This matches the Melee Dolphin `OSContext.c` `OSSwitchFiber(pc, newsp)`
  implementation and the standard GX OSContext layout.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
