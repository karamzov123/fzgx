# Finding 47 — ADXT sine/cosine table semantic name

- Rename: `lbl_8012B938` -> `adxt_sincos_table`
- Address/size: `.data` `0x8012B938`, `0x2080` bytes (8320 bytes)
- Evidence: findings/44 identifies this as a 1024-entry cosine/sine pair lookup table with quarter-wave symmetry. `src/game/adxt_80053A30.c` imports the complete 8320-byte object and indexes it in the ADXT processing path at 0x80053D44, using a 0x2000-byte half-table offset. The consumer and table family identify it as an ADXT trigonometric lookup table rather than an untyped label.
- Change is symbolic-only via `tools/rename_sym.py`; no instruction or layout change intended.
- Verification: fresh `python3 configure.py && ninja -j22` completed successfully; `build/GFZE01/main.dol` SHA-1 is `421c88106697d3275a3fc26fb7a01bf6d816b271`.
