# Finding 259 — natc_compile lacked built-in `--self-test` (house-pattern gap)

**Date:** 2026-08-26  **Worker:** tool  **Severity:** test-coverage / silent-failure risk

## Symptom
A tool-health sweep of the 5 core tools showed `natc_compile.py` was the ONLY
one without a built-in `--self-test` (the house pattern: find_xrefs,
emit_m2c_asm, natc_loop, natc_preflight, natc_gate all print `SELF-TEST OK`
in-process). `natc_compile` is the tool conversion workers invoke on EVERY
attempt, so a regression there is the highest-blast-radius silent failure.

## Why this matters
`natc_compile.main()` assembles the mwcc argv from the authoritative ninja
edge (`canonical_command`) and rewrites `-c` to the candidate + sets `-o` to a
content-addressed cache path. If `canonical_command()` mis-parses (e.g. drops
`mwcceppc.exe`, or the `-c`/`-o` substitution breaks), a candidate is compiled
under WRONG FLAGS and scores 0 — indistinguishable from a worker plateau. With
no in-process check, that class of bug could only be caught by an expensive
live build, not by the fast self-test sweep.

## Fix (`tools/natc_compile.py`)
- Extracted `build_argv(canon, src, out_obj)` (the `-c`/`-o` assembly) so
  `main()` and `self_test()` share ONE implementation — no drift between the
  live path and the check.
- Added `self_test()`: imports the module's own `load_unit`/`canonical_command`
  for a real carved unit (`main/dolphin/os/OSError`), asserts the canon edge
  shape (`mwcceppc.exe` present, `mw_version` resolved, `-c` present / `-o`
  absent), then runs `build_argv` on a temp candidate and asserts `-c` points
  at the candidate, `-o` at the cache path, and `mwcceppc.exe` survives.
  **Does NOT invoke MWCC** (builds are the integrator's job; contract forbids
  tooling workers from building).
- Added `--self-test` flag; relaxed `--unit`/`--src` to non-required and
  re-added the manual requirement check in the non-self-test branch so the
  flag works standalone.

## Verification
- `natc_compile --self-test` → `SELF-TEST OK (canon edge found; -c/-o assembly
  correct for main/dolphin/os/OSError)`.
- Real compile of a candidate on `main/dolphin/os/OSError` still runs (clean
  `duplicate_candidate` refusal from the prior telemetry ledger — correct).
- Added `tests/test_natc_compile_selftest.py` (3 fixtures: self-test passes,
  runs without invoking mwcc, missing args error outside self-test).
- Full repo suite: **80 passed** (was 77; +3), no regressions.

## Status
All 5 core tools now have a built-in `--self-test`. The tool-health sweep is
green across the board.
