# Finding 69 — AI DMA start/stop helpers

Symbolic-only rename wave on `contrib-20260825-ai-start-stop`.

- `fn_8001DFCC` -> `AIStartDMA`
  - Sets bit `0x8000` in the AI control register at `0xCC005036`.
- `fn_8001DFE4` -> `AIStopDMA`
  - Clears the same control bit.

The bodies match the Melee Dolphin `ai.c` `AIStartDMA` and `AIStopDMA`
implementations exactly at the register-operation level. No logic or layout
was changed.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
