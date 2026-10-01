# Finding 61 — current-thread getter

Symbolic-only rename on `contrib-20260825-os-current-thread`.

- `fn_800102AC` -> `OSGetCurrentThread`
- The function loads the OS current-thread pointer from the fixed OS global
  slot at `0x800000E4` and returns it.
- This matches the Melee Dolphin `OSThread.c` implementation of
  `OSGetCurrentThread`, which returns the current-thread global directly.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
