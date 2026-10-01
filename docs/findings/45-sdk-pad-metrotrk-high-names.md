# Finding 45 — SDK PAD + MetroTRK HIGH rename wave

Applied on contributor branch `contrib-20260823-wave` via `tools/rename_sym.py`.
Evidence is the verbatim structural correspondence in `/tmp/naming-study-sdk-fns.md`
against Melee SDK pad.c / MetroTRK msghndlr.c, plus current GFZE01 caller/source
bodies. All are symbolic-only renames.

| VA | old | new |
|---|---|---|
| 8001DBC8 | fn_8001DBC8 | PADSetAnalogMode |
| 8001DDD0 | fn_8001DDD0 | SamplingHandler |
| 8001DE30 | fn_8001DE30 | PADSetSamplingCallback |
| 80089C5C | fn_80089C5C | TRKDoReadRegisters |
| 80089EEC | fn_80089EEC | TRKDoWriteRegisters |
| 8008A1CC | fn_8008A1CC | TRKDoReadMemory |
| 8008A3C0 | fn_8008A3C0 | TRKDoWriteMemory |
| 8008A66C | fn_8008A66C | TRKDoDisconnect |
| 8008AFF0 | fn_8008AFF0 | TRK_flush_cache |

Verification: deleted `build/GFZE01/main.dol`, ran fresh `ninja`, exit 0;
`build/GFZE01/main.dol` SHA-1 =
`421c88106697d3275a3fc26fb7a01bf6d816b271`. Progress and matched counts
unchanged as expected for byte-neutral renames.
