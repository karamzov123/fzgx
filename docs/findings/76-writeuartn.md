# Finding 76 — WriteUARTN

Symbolic-only rename on `contrib-20260825-writeuartn`.

- `fn_8001581C` -> `WriteUARTN`
- Direct body evidence from the precomputed UART queue:
  - requires the UART initialization magic/state;
  - converts newline bytes to carriage-return/newline form;
  - performs the per-byte EXI transaction against the UART device;
  - returns the writer status/count according to the UART protocol.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
