# Finding 79 — arctangent data carve

Split-safe symbolic data wave on `contrib-20260825-atan-data-carve`.

Replaced the single overlapping `lbl_801327F8` object span with four contiguous,
non-overlapping objects preserving every original address:

- `lbl_801327F8`: `0x801327F8`, size `0x10028`
- `atan_taylor_coeff_table`: `0x80142820`, size `0x20`
- `lbl_80142840`: `0x80142840`, size `0x10020` (unclassified gap)
- `atan_to_binangle_table`: `0x80152860`, size `0x8000`

Evidence for the two semantic labels comes from direct consumers in the
arctangent routine `fn_8006D2AC`:

- The Taylor table contains eight coefficients beginning at π/4 and is loaded
  by the small-argument reduction path.
- The binary-angle table contains 1024 pairs used by the indexed multiply/add
  and integer conversion path; its first scale value is 65536/(2π).

The gap object preserves the complete original data coverage while avoiding
DTK overlap errors. No bytes or logic changed.

Verification: configure.py and one fresh Ninja wave completed successfully;
resulting `build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
