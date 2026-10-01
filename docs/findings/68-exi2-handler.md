# Finding 68 — EXI2 interrupt-handler installer

Symbolic-only rename on `contrib-20260825-exi2-handler`.

- `fn_8008ED30` -> `EXI2_SetInterruptHandler`
- The body masks the EXI2-related interrupt, installs the callback at
  interrupt `0x19`, then unmasks the relevant interrupt bit. The handler
  reference and interrupt sequence match the Melee MetroTRK EXI2 support
  interface.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
