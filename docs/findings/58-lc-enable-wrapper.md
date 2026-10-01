# Finding 58 — LCEnable wrapper

Symbolic-only rename on `contrib-20260824-lc-enable`.

- `fn_8000B804` -> `LCEnable`
- Direct Melee `OSCache.c` correspondence: the wrapper disables interrupts,
  invokes `__LCEnable`, restores the prior interrupt level, and returns.
  The adjacent `__LCEnable` was named in finding 56.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
