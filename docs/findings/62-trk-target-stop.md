# Finding 62 — MetroTRK target stop helper

Symbolic-only rename on `contrib-20260825-trk-target-stop`.

- `fn_8008B484` -> `TRKTargetStop`
- The body stores `1` into `gTRKState + 0x98` (the stopped flag), returns
  zero, and has no other behavior.
- This is an exact semantic match to Melee MetroTRK `TRKTargetStop()`, which
  calls `TRKTargetSetStopped(true)` and returns `kNoError`.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
