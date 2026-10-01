# Finding 50 — ADXT/SVM assert-string-backed function names (wave)

Branch `contrib-20260824-adxt-names`, commit `b3f82a9`. Symbolic-only via
`tools/rename_sym.py`; fresh `configure.py --dtk build/tools/dtk && ninja -j22`
gate GREEN, post-link SHA-1 `421c88106697d3275a3fc26fb7a01bf6d816b271` (exact).

Mined from findings/39 (string-backed table) + findings/41/48 methodology.
Names already taken by earlier waves were suffixed `_2` / `_family` to avoid
collision with the distinct retail functions that own the canonical names.

| VA | old | new | evidence |
|---|---|---|---|
| 80059428 | fn_80059428 | SVM_SetCbSvr_2 | consumes `_SVM_SetCbSvr_too_many_server_function_str` in-unit; caller tail_800410A4 registers it as server callback alongside SVM_DelCbSvr. Name taken at 0x80059DD8. |
| 8004DBBC | fn_8004DBBC | adxt_trap_entry_not_enough_data | sole caller is ADXT_ExecHndl's decode path; body asserts E8101201 "not enough data" |
| 8004F818 | fn_8004F818 | ADXF_SetPtdId_family | validates ptid/flid ranges against E9040828 ptid/flid range strings, then indexes the ADXF handle table |
| 8004FC94 | fn_8004FC94 | ADXF_Stop_family | asserts E9040822/E9040823 "'pxf->stm' is NULL.(ADXF_Stop)"; name ADXF_Stop already assigned at 0x80050698 |

Verification output:
```
ninja: SDK Code 97.52% fuzzy, 39.74% matched, 100.00% linked
sha1sum build/GFZE01/main.dol → 421c88106697d3275a3fc26fb7a01bf6d816b271
```
