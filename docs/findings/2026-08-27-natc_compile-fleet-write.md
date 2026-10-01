# natc_compile fleet-state write paths — 2026-08-27 (fleet/tool, closure)

This cycle traced exactly *where* a live conversion attempt writes fleet state,
to settle whether the tooling worker can run a real MWCC compile+score without
side effects. Conclusion: **it cannot** — every compile path touches RUNS_DB.
This hardens (with code-level evidence) the prior assessment that the live
score is integration/conversion-worker domain only.

## Where natc_compile writes RUNS_DB
`tools/natc_compile.py`:
- L40 `RUNS_DB = ~/.cache/natc/runs.sqlite`
- L549 `if this_kind == "attempt" and not args.over_budget:` ->
  `identical_attempt_exists()` (L551, ro connect) then `log_refusal()` (L560,
  write) on duplicate.
- L690-707 SECOND gate: `allow_over_budget = (args.over_budget and
  os.environ.get("NATC_ALLOW_OVER_BUDGET")=="1")`. If not allowed and
  `this_kind=="attempt"`, it `log_refusal(...)` (L707, WRITE) and exits.
  So `--over-budget` alone does NOT skip the ledger — it needs the env flag.
- L728/809/830/855: the actual attempt logging / budget bookkeeping all
  `sqlite3.connect(RUNS_DB)` and `.commit()`.

## Implication
There is **no side-effect-free invocation** of a real MWCC compile+score:
- `attempt_kind(args)` returns "attempt" whenever a candidate `--src` is
  supplied; that branch always reaches a RUNS_DB write (duplicate check,
  refusal, or success log).
- Therefore running `natc_compile --unit --src --symbol` would write to the
  shared attempt ledger — attributing an attempt to a worker, spending budget,
  and potentially poisoning a real worker's resume/ledger (the exact hazard
  the `--worker` requirement at natc_loop L554-559 warns about).

## What this means for the tooling worker
- The four authored tools (find_xrefs, similar, emit_m2c_asm, natc_loop) and
  `natc_compile`'s *command assembly* are fully verified (self-tests PASS,
  context assembly proven live ×2, MWCC binary reachable).
- The **actual numeric score** of a candidate is, by design, a
  conversion-worker action under a unit lease + budget (contract L108-110),
  and is correctly NOT performable by the tooling worker without violating
  L13/L143 and risking ledger poisoning.
- This is the final dimension of the tool-health sweep: the boundary between
  "tooling worker can verify" and "requires a conversion worker" is now
  precisely located at the RUNS_DB write in natc_compile — not at a missing
  binary or broken tool.

## Final tool-worker status (all dimensions closed)
1. 4 queue items: built, fixture-tested, live-verified, promoted to main,
   hard-tail ratified (77aba4a, reachable from fleet/hard).
2. Full `tools/` suite: 112 passing.
3. CLI `--self-test`: 7/7 runnable PASS.
4. `natc_harvest`: functional; 3 pkg-able GXGeometry fns staged for integ.
5. `health-state.json` stranded gauge: decoded as a fleet backlog metric.
6. Build tree: present, byte-identical to canonical, natc_metrics runnable.
7. Attempt-flow compile dependency: sound + reachable; live score correctly
   out-of-scope (RUNS_DB write path located + confirmed unavoidable).

No further tool-worker action exists. Blockers are external: integ must gate
the GXGeometry batch (L143); remaining stranded fns need worker re-conversion.
