# Finding 255 — natc_preflight refusal-evidence write crash (TOCTOU)

**Date:** 2026-08-26  **Worker:** tool  **Severity:** fleet-reliability (defensive)

## Symptom
The durable submission-queue dump contained ~40 batches whose `error` field was
a `FileNotFoundError` traceback from `natc_preflight.py` line ~167/176:

```
FileNotFoundError: [Errno 2] No such file or directory:
'/home/armandofm/.cache/natc/submissions/<worker>/<batch>/PREFLIGHT.md'
```

The traceback terminated the process with a non-zero, non-refusal rc, so the
batch was recorded as a *tool error* rather than a clean refusal.

## Root cause
`natc_preflight.main()` decides refusal, then writes `PREFLIGHT.md` into
`batch_dir`. The write was guarded only by `if batch_dir.is_dir():` checked
*before* the `write_text`. Between that check and the write the integrator's
gate can archive/move the batch (TOCTOU) — `is_dir()` was True, then the
`write_text` raises `FileNotFoundError`, which escaped as a traceback. The
refusal itself is correct and already printed to stderr; only the convenience
evidence file is affected.

## Fix (tools/natc_preflight.py)
Extracted `_write_refusal_evidence(batch_dir, failed)`:
- still writes `PREFLIGHT.md` when the dir is present (clean path unchanged —
  verified: a real refused batch dir gets its PREFLIGHT.md),
- on `OSError` (incl. `FileNotFoundError`) during the write, degrades to the
  stderr note we already printed — never raises, never a traceback.

Refusal *logic* in `natc_gate.py` is untouched. No `--self-test` coupling; the
fix is purely crash-proofing of an evidence write.

## Test
`tests/test_natc_preflight_evidence.py` (added):
- `test_missing_batch_dir_refuses_without_traceback` — subprocess on a
  nonexistent batch dir asserts no `Traceback` in stderr.
- `test_evidence_write_is_oerror_safe` — monkeypatches the helper with a dir
  whose `PREFLIGHT.md` write raises `FileNotFoundError`; asserts it swallows it.

Both pass under `uv run --with pytest`. Full repo suite: 73 passed (was 71;
+2).

## Verification
- `python3 tools/natc_preflight.py --self-test` (n/a; no self-test) — replaced
  by the new fixture + live run.
- Live: missing dir -> clean refusal, no traceback; real dir -> PREFLIGHT.md
  written. Confirmed both paths.
- Full suite `uv run --with capstone --with pytest python3 -m pytest tests/ -q`
  -> 73 passed.
