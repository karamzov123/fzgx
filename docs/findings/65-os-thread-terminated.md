# Finding 65 — thread termination predicate

Symbolic-only rename on `contrib-20260825-os-thread-terminated`.

- `fn_800102B8` -> `OSIsThreadTerminated`
- The body reads the thread state at offset `0x2C8` and returns true only for
  states `0` and `8`.
- This exactly matches the Melee Dolphin `OSThread.c` implementation of
  `OSIsThreadTerminated`.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
