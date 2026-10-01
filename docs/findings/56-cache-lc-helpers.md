# Finding 56 — OS cache-line (LC) helper names

Symbolic-only rename wave on `contrib-20260824-cache-lc`.

The four renamed functions are direct semantic matches to the corresponding
Melee Dolphin `dolphin/os/OSCache.c` helpers; GX bytes remain authoritative.

- `fn_8000B738` -> `__LCEnable`: enables locked-cache mode, touches the
  cached region, programs HID2/L2CR, and initializes the 512-line locked
  cache region. The helper is called by the existing interrupt-protected
  wrapper at `fn_8000B804`.
- `fn_8000B864` -> `LCLoadBlocks`: programs the locked-cache load transfer
  control registers from destination tag, source address, and block count.
- `fn_8000B888` -> `LCStoreBlocks`: paired store-transfer register setup.
- `fn_8000B8AC` -> `LCQueueWait`: spins until the locked-cache transfer queue
  has no more than the requested length pending.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
