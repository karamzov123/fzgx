# Finding 51 — ADXT AHX callback and SVM error helper names

Symbolic-only rename wave on contributor branch `contrib-20260824-adxt-ahx-callbacks`.

## Applied

- `fn_80051CC0` -> `ADXT_AHXExecCallback`
  - `ADXT_AttachAHX` stores this address into the AHX callback slot at 0x801A2A80.
  - Body drives the attached AHX stream: checks handle state, drains/executes
    frames through `fn_80053EB0`, updates consumed byte counters, and finalizes
    the stream through the AHX helper callbacks.
- `fn_80051DC4` -> `ADXT_AHXStopCallback`
  - `ADXT_AttachAHX` stores this address into the stop callback slot at
    0x80187E8C. Body calls `ADXT_Stop`, tears down the attached AHX stream,
    clears the owning handle's +0xB0 pointer, and invokes cleanup.
- `fn_800595A4` -> `SVM_ReportErrorString`
  - Copies the supplied error string into the SVM global error buffer
    `lbl_8018FEE0` with `strncpy(..., 0x7f)`, then invokes the registered SVM
    error callback through `lbl_8018FF60`. It is called by the ADXM worker
    error path when the worker reaches its boundary condition.

The four SVM handle field accessors at 0x80059CA4/0x80059D04/0x80059D68 remain
unnamed because their field semantics are not uniquely established by this
wave.

## Verification

Fresh `rm -f build/GFZE01/main.dol && python3 configure.py && ninja -j22`
completed with exit 0. SHA-1:
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
