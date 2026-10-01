# Tool-health sweep — 2026-08-27 (fleet/tool, pre hard-tail sign-off)

Run by tooling worker (`tool`) as a baseline for the hard-tail review of queue items 1–4.

## Command
```
cd ~/fzgx-wt/tool
uv run --with capstone --no-project python3 -m unittest discover -s tests -p 'test_*.py'
uv run --with capstone --with pytest  --no-project python3 -m pytest tests/test_carve_guard.py tests/test_natc_feedback.py
```

## Result
- unittest discover: 86 collected, **2 errors**. Both errors are import-time only:
  `tests/test_carve_guard.py` and `tests/test_natc_feedback.py` `import pytest`, which
  is absent from the capstone-only uv envelope. Those two files are **pytest** suites
  (`@pytest.fixture`, `pytest.raises`, `@pytest.mark.parametrize`), not unittest.
- The remaining 84 unittest tests PASS (includes test_find_xrefs, test_emit_m2c_asm,
  test_natc_loop, test_similar under the capstone envelope, plus the 14/18 others).
- The 2 pytest files run correctly under `uv run --with capstone --with pytest`:
  **20 passed in 0.11s**.

## Verdict
**Full `tools/` test suite is GREEN: 104 tests pass, 0 real failures.** The only
discovery-time "errors" are a runner mismatch (unittest cannot load pytest suites),
not a regression in any tooling item.

## Environment notes (for anyone re-running)
- capstone is NOT in the base interpreter (PEP 668, no system pip). `tools/similar.py`
  and `tools/natc_loop.py` shell out to `uv run --with capstone` for the disassembler.
- Mixed test runners: unittest for most `tests/test_*.py`; pytest for
  `test_carve_guard.py` / `test_natc_feedback.py`. Do NOT use `python3 -m unittest
  discover` alone as a green/red gate — it will report false errors on the pytest files.
- Worker tree has no build.ninja by policy (conversion workers + this tooling worker
  never build the DOL). Build-dependent suites (natc_compile/ninja) are expected to be
  exercised only by the integrator (`integ`) on the canonical tree.
