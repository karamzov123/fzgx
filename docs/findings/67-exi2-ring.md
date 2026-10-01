# Finding 67 — MetroTRK EXI2 ring-buffer interface

Symbolic-only rename wave on `contrib-20260825-exi2-ring`.

- `fn_8008E114` -> `EXI2_ReadN`
  - Reads and advances the receive ring, including wraparound copying and
    interrupt-protected accounting.
- `fn_8008E21C` -> `EXI2_WriteN`
  - Writes and advances the transmit ring with the corresponding wraparound
    and capacity checks.
- `fn_8008E324` -> `EXI2_Init`
  - Initializes the ring bases, capacities, positions, and callback state;
    call sites pass the EXI2 buffer regions and `0x800` size.
- `fn_8008E374` -> `EXI2_Poll`
  - Returns the queued-byte count from the initialized EXI2 state.

The signatures and call graph match the Melee MetroTRK EXI2 interface; the
GX bodies are preserved exactly and only symbols/call-site references change.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
