# Finding 54 — AI DMA callback/setup names

Symbolic-only rename wave on `contrib-20260824-ai-dma`.

- `fn_8001DF00` -> `AIRegisterDMACallback`: saves and replaces the callback
  global consumed by `__AIDHandler`, returning the previous callback under
  interrupt exclusion. This matches the Melee Dolphin AI API exactly.
- `fn_8001DF44` -> `AIInitDMA`: programs the AI/DSP DMA start-address and
  length registers under interrupt exclusion. The register sequence matches
  the Melee `AIInitDMA(start_addr, length)` implementation.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
