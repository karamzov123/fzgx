# Finding 256 — natc_gate.completeness_check crashed (undefined `cand_defs`/`dest_defs`)

**Date:** 2026-08-26  **Worker:** tool  **Severity:** gate correctness (fail-closed → actually crash)

## Symptom
`tools/natc_gate.py --self-test` raised:
```
NameError: name 'cand_defs' is not defined
```
at `completeness_check()` line 357, and `tests/test_completeness_presence.py`
had 4 failures (all the same NameError). The gate's completeness check — which
exists to refuse partial-file submissions that would silently delete sibling
asm/C definitions from the destination — was crash-on-call, so it could never
actually run. The check is fail-closed by crashing, but a crash is not a clean
refusal and it corrupts the gate run (any batch hitting completeness_check
instead of getting a proper PARTIAL-FILE message).

## Root cause
In `completeness_check()` (per-candidate loop) the code read the candidate and
destination text, computed `dest_asm = asm_defs(dest_text)` and
`defined = def_names(text)`, then immediately referenced two bindings that were
**never assigned**:
- `cand_defs` — intended = names the candidate defines = `def_names(text)`
- `dest_defs` — intended = names the current head defines = `def_names(dest_text)`

The `present()` helper also closed over the now-removed `defined`. The four
completeness tests exercised exactly this path, so they went red the moment the
bug was introduced.

## Fix (tools/natc_gate.py, ~line 348)
Replaced `defined = def_names(text)` / `present()` closure with the two intended
bindings:
```python
cand_defs = def_names(text)        # names the candidate defines
dest_defs = def_names(dest_text)   # names the current head defines

def present(name):
    return name in cand_defs
```
No refusal-logic change: the PARTIAL-FILE messages and the `removes:` ack
mechanism are untouched. This only makes the check run instead of crash.

## Verification
- `natc_gate.py --self-test` PASSED (poisoned candidate refused, tree restored).
  Post-test rebuild half is BY DESIGN run only in the integrator tree (worker
  trees have no build.ninja) — matches the 2026-08-25/26 board history.
- `tests/test_completeness_presence.py`: 4 passed (was 4 failed).
- Full repo suite: 73 passed (was 69 + 4 red, now 73 green), no regressions.

## Rule compliance
Per assignment rule, re-ran `--self-test` after any gate change (both the
poisoned-input refuse path and the clean path exercised by the suite).
Refusal semantics unchanged. `.gateorig` backups untouched.
