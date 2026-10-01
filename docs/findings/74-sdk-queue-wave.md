# Finding 74 — SDK queue wave: SI, EXI, and quaternion matrix

Batched symbolic-only queue wave on `contrib-20260825-sdk-queue`.

- `fn_8006E498` -> `C_MTXQuat`
  - Exact quaternion-to-matrix scalar arithmetic: quaternion component loads,
    normalization scale `2/(q·q)`, and the 3x4 matrix stores match the Melee
    MTX implementation.
- `fn_8001193C` -> `SIIsChanBusy`
  - Exact SI channel request predicate: tests the channel packet command and
    global SI ownership state.
- `fn_80015464` -> `__EXIGetID`
  - Exact EXI device-ID transaction: attaches EXI0, locks/selects the channel,
    issues the ID command, reads the four-byte result, and releases the bus.

All three names came from the precomputed supervisor scout queue and were
filtered against the current symbol table before application. No logic changed.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
