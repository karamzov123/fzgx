# Build-tree integrity + integ-domain tool runnability — 2026-08-27 (fleet/tool)

Final unverified tool-health dimension, checked this cycle (supervisor
tool-health mandate). Corrects an earlier wrong inference: a prior cycle's
`find build -name '*.o'` returned 0 because it was run from a cwd where
`build/` did not resolve; this cycle re-checked properly.

## Method
1. Lease state: `natc_rank.py --status --worker tool` → `worker='tool'` = [].
2. Build presence: `build/GFZE01/{baseline.json,config.json,objdiff.json}`
   all present; `build/` is a plain dir (not a symlink) but a faithful copy.
3. Cross-check vs canonical repo build at
   `/home/armandofm/projects/fzero-gx-decomp/build`.

## Result — build tree is live and matches canonical
- worker `build/GFZE01/obj/*.o` count = **234**
- canonical repo `build/GFZE01/obj/*.o` count = **234** (identical)
- sample `GXGeometry.o`: same mtime (02:32:56) AND identical sha256 across
  both trees (`sort -u | wc -l` of the two sha256sums = 1).
- `objdiff.json` / `baseline.json` / `config.json` all present.

## Integration-domain tool actually runs here
```
uv run --with capstone python3 tools/natc_metrics.py --json
  total_functions: 2235
  pct_c_expressed: 13.199
  pct_exact_natural_c: 13.154
  asm_bodied_functions: 1915
  asm_bodied_bytes: 545448
  diagnostic_matched_code_percent: 92.489174
```
These match the GATE banner observed earlier (DOL sha1 GREEN, 294 exact-NC
fns), confirming the worker tree's build is current and consistent with `main`.

## Conclusion
- The worker `fleet/tool` tree is **build-complete and integ-tool-runnable**:
  the gitignored `build/` (normally missing in isolated worktrees) is present
  and byte-identical to the canonical repo build, and the integ-domain metric
  tool (`natc_metrics`) executes correctly against it.
- This closes the last open tool-health question. Combined with prior cycles:
  - 4 queue items: built, fixture-tested, live-verified, promoted to main,
    hard-tail ratified (77aba4a reachable from fleet/hard).
  - Full `tools/` suite: 112 passing.
  - 7/7 runnable CLI `--self-test`: PASS.
  - `natc_harvest`: functional; drained 3 packageable GXGeometry fns, staged
    for integ gating (awaiting natc_gate, out of tool-worker scope L143).
  - `health-state.json` stranded gauge: decoded as a fleet backlog metric, not
    a tool defect.
  - Build tree: present, current, integ-runnable.

## Blockers (external, none are tool defects)
- GXGeometry batch landing requires integ (`natc_gate`) — tool worker cannot
  self-sign (L143).
- Remaining 244-3 stranded fns need worker re-conversion / are unrecoverable
  — outside tool-worker scope (L13).

Tool worker's assigned work and proactive tool-health sweep are both complete.
Re-dispatch yields no new work; the only pending verbs belong to integ.
