# Integ delivery packet — fleet/tool tooling merge (2026-08-27)

Tooling worker's handoff so integ can promote the `fleet/tool` branch to
`main` in ONE pass. All evidence below is live and was captured this cycle.

## Scope (what merges)
Pure fleet-shared infra. **No `src/` edits, no candidate `.c`, no gate-refusal
source changes** — the gate owns canonical src, these tools are fleet infra.
Unpromoted vs main (additive only):
- `tools/tool_health_sweep.py`            (item 6; new proactive self-test harness)
- `tools/sda21_fix.py`                    (refactored + now has --self-test + fixture)
- `tests/test_tool_health_sweep.py`       (fixture for item 6)
- `tests/test_sda21_fix.py`               (fixture for sda21_fix)
- `tests/test_natc_compile.py`            (NEW fixture — closed NO-FIXTURE gap)
- `tests/test_natc_gate.py`               (NEW fixture — closed NO-FIXTURE gap)
- `tests/test_natc_preflight.py`          (NEW fixture — closed NO-FIXTURE gap)
- `tests/test_find_slice_unit.py`         (NEW fixture — closed NO-FIXTURE gap)
- `tools/REVIEW-queue-1-4.md`             (item 6 ratify handoff; append only)
- `findings/2026-08-27-*`                 (audit docs; no code)

## Pre-approval (run in canonical tree, any order)
```
cd ~/projects/fzero-gx-decomp
# 1. dry-merge is clean (no CONFLICT lines):
git merge-tree $(git merge-base main fleet/tool) main fleet/tool | grep -i conflict || echo CLEAN
# 2. full tool-test suite on fleet/tool, then on main for regression compare:
uv run --with capstone --with pytest --no-project python3 -m pytest tests/ -q
#    fleet/tool: 143 passed ; current main: 116 passed (my 27 fixtures add the delta)
# 3. live health sweep on fleet/tool:
uv run --with capstone --with pytest --no-project python3 tools/tool_health_sweep.py
#    expect: ok=10 skip=1 no_fixture=0 fail=0
```

## What integ does
```
git checkout main && git merge fleet/tool        # additive; dry-merge was clean
# no build needed: tooling only, no src/ change. Re-run the suite to confirm:
uv run --with capstone --with pytest --no-project python3 -m pytest tests/ -q
```

## Sign-off gate (do NOT merge if)
- `git merge-tree` reports a CONFLICT (none observed this cycle).
- full suite on merged main drops below 143 (regression).
- `tool_health_sweep.py` reports any `FAIL` (red) — `NO-FIXTURE` is yellow, ok.

## Verification already done by tooling worker (this cycle)
- Lease state: `tool` holds 0 units (no src lease to conflict with conversion).
- fleet/tool full suite: **143 passed**.
- canonical main full suite (baseline, pre-merge): **116 passed**.
- live health sweep: `ok=10 skip=1 no_fixture=0 fail=0`.
- dry merge: clean (no CONFLICT).

This is a request for integ delivery, not a claimed merge. The tooling worker
does not self-merge (release rule: hard-tail ratify + integ deliver).
