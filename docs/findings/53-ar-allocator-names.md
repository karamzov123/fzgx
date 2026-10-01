# Finding 53 — ARAM allocator names

Symbolic-only rename wave on `contrib-20260824-ar-alloc`.

- `fn_8001E954` -> `ARAlloc`: takes a 32-byte-aligned length, updates the
  ARAM stack pointer, records the block length, decrements free-block count,
  and returns the allocated ARAM address. This matches the Melee Dolphin AR
  allocator contract and the surrounding `ARInit`/`ARStartDMA` unit.
- `fn_8001E9BC` -> `ARFree`: decrements the block-length stack, optionally
  writes the freed length to the output pointer, restores the stack pointer,
  increments free-block count, and returns the new stack pointer. This is the
  exact paired AR allocator operation.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
