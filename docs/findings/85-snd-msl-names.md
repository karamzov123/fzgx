# Finding 85 — sound archive and MSL numeric names

Batched symbolic-only wave on `contrib-20260825-snd-msl-names`.

Applied three cached direct matches:

- `fn_80066C14` -> `SndLoadSoundArchive`
  - Validates the `cgax`/`TDPK` archive header and channel range before
    driving archive/procedure loading.
- `fn_80083E84` -> `atof`
  - MSL string-conversion entry that routes through the floating conversion
    core and returns a double-valued result.
- `fn_80084F1C` -> `atoi`
  - Passes base 10 to the integer conversion core and applies the MSL
    LONG_MIN/LONG_MAX overflow handling and ERANGE behavior.

The sound body and both MSL routines are directly supported by the cached
scout disassembly and neighboring SDK/MSL cluster topology. `TRK_memcpy` was
not changed because its address already uses the repository's collision-
disambiguated `fn_800035C0_memcpy` form.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
