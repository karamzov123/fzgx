# Finding 48 — model/GX cached-register setter names

Applied on contributor branch `contrib-20260824-mtx-cache` via `tools/rename_sym.py`.
The five targets are the high-fan-in cached GX-register setters identified by
findings/41. Each compares an incoming packed parameter record against a
per-object shadow slot and writes the GX FIFO register only when changed.

| VA | old | new | evidence |
|---|---|---|---|
| 80072C24 | fn_80072C24 | ModelSetCachedParam_F0 | 4-word shadow record at object + 0xF0; write-through via GX register helper |
| 80072CC4 | fn_80072CC4 | ModelSetCachedParam_1F0 | 4-word shadow record at object + 0x1F0 |
| 80072D64 | fn_80072D64 | ModelSetCachedParam_2F0 | 5-field record at object + 0x2F0, including byte field |
| 80072E20 | fn_80072E20 | ModelSetCachedParam_430 | 5-field record at object + 0x430 |
| 800734A8 | fn_800734A8 | ModelSetCachedMaterial_570 | material/light shadow at +0x570 with +0x720 auxiliary table and 0xFF sentinel |

These functions have 15 callers each (findings/37), so the names improve
high-fan-in linkage readability. Changes are symbolic-only; no instruction or
layout changes were intended.

Verification: deleted generated `build/GFZE01/main.dol`, ran fresh
`python3 configure.py && ninja -j22`, exit 0. SHA-1:
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
