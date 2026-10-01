# Finding 70 — GX channel material color setter

Symbolic-only rename wave on `contrib-20260825-gx-chan-mat`.

- `fn_80035828` -> `GXSetChanMatColor`
- Direct body evidence:
  - dispatches the supported channel IDs;
  - packs the supplied RGBA color into the channel material register;
  - emits XF register `0x100C + channel` through the GP FIFO;
  - updates the corresponding GX material-color state.
- `GXInit.c` calls it for the two default material channels, and the body
  matches the Melee Dolphin `GXLight.c` `GXSetChanMatColor` implementation.
- This is the PM-priority high-fan-in target with 43 callers in the current
  fan-in inventory.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
