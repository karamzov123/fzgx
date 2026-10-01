# Finding 87 — SI response synchronization

Symbolic-only SI wave on `contrib-20260825-si-response-sync`.

- `fn_800137C4` -> `SIGetResponseSync`
- The helper disables interrupts, claims the per-channel SI packet, obtains
  status and response data, unpacks the response into the caller's output
  structure, translates the negative error cases, and restores interrupts.
- Its `(chan, output)` signature and packet-stride arithmetic match the
  synchronous SI response helper in the Dolphin SDK. The asynchronous/public
  `SIGetTypeAsync` path is separate and already named.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
