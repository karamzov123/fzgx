# Finding 55 — SVM status accessors

Symbolic-only rename wave on `contrib-20260824-svm-status`.

- `fn_80059CA4` -> `SVM_GetStatusU32`: NULL-checks the SVM handle and returns
  the 32-bit field at handle offset `+4`; the NULL path reports using the
  existing `E0040301 handl_is_null` diagnostic and returns zero.
- `fn_80059D04` -> `SVM_GetStatusS8`: NULL-checks the handle and returns the
  signed byte at handle offset `+1`; the NULL path uses the existing
  `E0092912 handl_is_null` diagnostic and returns zero.
- `fn_80059D68` -> `SVM_ClearStatus`: NULL-checks the handle, then takes the
  SVM server lock, clears the byte at handle offset `+1`, and unlocks.

These names are limited to the operations proven by the instruction bodies;
no ABI or broader SVM state interpretation is asserted.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
