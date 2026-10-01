# Finding 52 — MetroTRK MSR accessors

Symbolic-only rename wave on `contrib-20260824-trk-msr`.

- `fn_8008B0E0` -> `__TRK_get_MSR`: exact body is `mfmsr r3; blr`.
- `fn_8008B0E8` -> `__TRK_set_MSR`: exact body is `mtmsr r3; blr`.

The pair is used by MetroTRK interrupt/context code in
`dolphin/metrotrk/trk_80088B00.c` and `dolphin/metrotrk/main.c`. Names match the
PowerPC/MSR accessor semantics and do not claim more than the instruction
bodies prove.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
