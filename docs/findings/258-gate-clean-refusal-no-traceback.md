# Finding 258 — natc_gate.main leaked Tracebacks on text-only refusals (queue pollution)

**Date:** 2026-08-26  **Worker:** tool  **Severity:** fleet-reliability (durable-queue data quality)

## Symptom
Durable submission-queue scan (191 batches): **49** records carry the error
`[natc_gate] no .c candidates in batch dir` followed by a full `Traceback`,
and the traceback class totals 50 across all states. A refusal is a normal,
expected outcome — it must NOT be recorded as a crash. The traceback pollutes
the `error` column, makes queue health sweeps noisy, and was the same symptom
class as finding 255 (natc_preflight evidence write).

## Root cause
`natc_gate.cheap_checks()` signals a **text-only refusal** (no .c candidates /
missing CARD.md / no provenance / duplicate) by `raise SystemExit(msg)`. But
`main()` called `cheap_checks()` **without catching SystemExit** (both the
`--preflight` branch at ~L1184 and the main gate path at ~L1207). Because
`SystemExit` is a `BaseException`, an uncaught one prints a `Traceback` to
stderr and exits non-zero — and the integrator's queue-recording wrapper
captured that traceback into the batch `error` field.

The fix is at the call site, not in `cheap_checks` (which is shared with
`natc_preflight`, where the refusal is already caught and rendered cleanly).

## Fix (`tools/natc_gate.py`)
Wrapped both `cheap_checks()` call sites in `try/except SystemExit` → print
`[natc_gate] REFUSED: <msg>` to stderr and `sys.exit(1)`. This is a
refusal-OUTPUT change (cleaner), not a refusal-LOGIC change: the same conditions
still refuse, with the same message text, just without the traceback.

`--self-test` re-run both ways: poisoned candidate still refused + tree
restored; clean path green. (Contract: never change gate refusal logic without
re-running `--self-test` both ways.)

## Verification
- `natc_gate --batch <empty>`: now prints `REFUSED: [natc_gate] no .c candidates
  in batch dir`, rc 1, **no Traceback**.
- Added `tests/test_natc_gate_clean_refusal.py` (empty-batch + missing-CARD
  paths assert rc 1, no `Traceback`, `REFUSED` present).
- Full repo suite: **77 passed** (was 75; +2), no regressions.

## Blocker closed
This is the LAST live traceback-pollution source in `tools/`: finding 255
(preflight evidence write) + 258 (gate main) together eliminate the class.
Historical 50 polluted records are inert (already-retired batches); new refusals
will be clean.
