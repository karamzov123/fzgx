# Finding 59 — MetroTRK target PC getter

Symbolic-only rename on `contrib-20260825-trk-getpc`.

- `fn_8008B6BC` -> `TRKTargetGetPC`
- Body loads `gTRKCPUState + 0x80` and returns it.
- The Melee MetroTRK `Default_PPC` layout identifies offset `0x80` as the
  saved PowerPC program counter (`Default.PC`), and the helper is used by the
  target-step/support path.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
