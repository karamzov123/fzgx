# Finding 81 — AXMix device-control clear

Symbolic-only rename on `contrib-20260825-axmix-clear`.

- `fn_80026D70` -> `axmix_device_ctrl_clear`
- The helper reads the device/handle index at offset `0x18`, indexes the
  64-entry AXMix control table at `lbl_80176160` with its `0x60`-byte stride,
  and clears the selected control entry's field at offset `0`.
- The role is supported by the surrounding AXMix accessor family and callers
  in ADXT, AXMix, sasnd, and sound-allocation paths. A descriptive internal
  name is used deliberately; no public SDK name is asserted.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
