# Finding 257 — natc_preflight lacked built-in `--self-test` (house-pattern gap)

**Date:** 2026-08-26  **Worker:** tool  **Severity:** test-coverage / regression-blindness

## Symptom
A tool-health sweep (continuation of findings 255/256) showed `natc_preflight.py`
was the only core tool without a built-in `--self-test`. The other three core
tools (`find_xrefs`, `emit_m2c_asm`, `natc_loop`, `similar`) all print
`SELF-TEST OK` from an in-process check; preflight had none, so a regression in
its refusal path (exactly the finding-255 class: traceback on evidence write)
would only be caught by the slower pytest path, and only if that path happened
to run.

## Change (tools/natc_preflight.py)
- Added `self_test()` exercising the two core invariants on temp dirs (no tree
  touch): (1) missing-batch dir refuses WITHOUT a traceback, (2) a clean-path
  batch runs the refusal backend without crashing.
- Extracted `run_one(batch_dir, worker, min_batch)` as the shared back end that
  returns the same rc `main()` would (0 clean / 1 refused / 2+ tool error),
  without the argparse layer, so the self-test reuses the exact logic.
- Moved argparse into `__main__` and changed `main()` to take a parsed `args`
  object (`main(args)`), so `--self-test` and the normal `--batch` path share one
  parser. No behavior change to the `--batch` refusal output.
- Fixed a latent bug in `run_one`: a CLEAN pass (cheap_checks returns normally,
  `failed is None`) was being misclassified as "refused with no message" because
  the `if not failed` guard ran even when no SystemExit occurred. Now the
  "no message" bug-string is only set inside the `except SystemExit` block, so a
  clean batch correctly returns rc 0.

## Verification
- `natc_preflight.py --self-test` → `SELF-TEST OK (missing-batch no-traceback;
  clean-batch no-crash)`.
- Real `--batch /tmp/none` path still refuses cleanly, no traceback.
- `tests/test_natc_preflight_selftest.py` added (2 fixtures: self-test OK; legacy
  `--batch` path still refuses without traceback).
- Full repo suite: 75 passed (was 73; +2), no regressions.

## Compliance
Refusal semantics unchanged (rc 0/1 contract preserved). `.gateorig` / gate
refusal logic untouched. No unit lease touched.
