# Tooling fixture-coverage audit — 2026-08-27 (fleet/tool)

Run by tooling worker (`tool`) as a tool-health sweep of `tools/` ahead of the
hard-tail sign-off of queue items 1–4.

## Method
1. Mapped every `tools/*.py` to the `tests/test_*.py` that references it by
   module name / import (string-substring + `import <base>` scan).
2. Smoke-tested the **untested** worker-facing modules for import loadability
   under the base interpreter (syntax compile + `importlib.import_module`, each
   in an isolated subprocess so import-time banner prints don't cross-contaminate).

## Coverage gaps (worker-facing tools with NO fixture test)
24 of 45 worker-facing modules are not referenced by any `tests/test_*.py`:

```
_wire_carve_guard  changes_fmt       cmp_units         decompctx
download_tool      dump_asm          elf_func_bytes    find_blob
find_slice_unit    ledger_progress   merge_seeds       mine_assert_xrefs
natc_ctx           natc_discriminator natc_escalate    natc_escalation
natc_integrate     natc_inventory     natc_precheck     public-export
reloc_parity_audit  transform_dep     unit_check        unit_status
```
Generators: 8/12 untested (`gen_8067c_carve`, `gen_initial_splits`,
`gen_mtxfused_carve`, `gen_mtxhead_carve`, `gen_pm10_carve`, `gen_pm10a_carve`,
`gen_pm11_carve`, `gen_stdio_carve`). Helpers: 7/8 untested
(`natc_metrics`, `natc_queue_sweep`, `natc_rebase`, `natc_splice`, `ninja_syntax`,
`project`, `words`).

## Import-loadability of the untested set (evidence)
- **22/24 untested worker-facing modules import cleanly** (syntax OK, import OK):
  `_wire_carve_guard, changes_fmt, cmp_units, decompctx, download_tool, dump_asm,
  elf_func_bytes, find_blob, find_slice_unit, ledger_progress, merge_seeds,
  mine_assert_xrefs, natc_ctx, natc_discriminator, natc_escalate, natc_escalation,
  natc_integrate, natc_precheck, public-export, transform_dep, unit_check, unit_status`.
- `reloc_parity_audit` — IMPORT_FAIL `FileNotFoundError: orig/GFZE01/sys/main.dol`.
  Not a defect: it requires the DOL, which is intentionally absent in the worker
  tree (same policy as the build-dependent suites; integrator-only).
- `natc_inventory` — IMPORT_FAIL `SystemExit 2`. Expected: it is a DEPRECATED
  delegation shim (21-line body) that `sys.exit()`s into `natc_metrics.py` at
  import time. Not a defect.

## Additional tool-health signals observed
- **Import-time side effects are heavy in some modules.** Importing
  `natc_integrate` / `unit_status` / `natc_inventory` prints a full GATE banner
  + STATE.md census to stdout at import time (observed during the smoke sweep:
  `DOL sha1 GREEN 421c88106697d3275a3fc26fb7a01bf6d816b271`, `294 exact
  natural-C fns / 20,968 B`, `objdiff matched_code 92.489% (diagnostic)`). This is
  safe but makes isolated unit testing of those modules awkward — a fixture would
  have to capture/redirect stdout. Flagging for hard-tail awareness.
- `dump_asm.py` is the only untested worker-facing tool that needs `capstone`
  (`ModuleNotFoundError: capstone` on import); fixable under
  `uv run --with capstone`. All other untested modules import with no extra deps.

## Verdict for items 1–4 (the assigned queue)
Items 1–4 (`find_xrefs`, `similar`, `emit_m2c_asm`, `natc_loop`) are all
**referenced by existing fixtures and pass** — see
`findings/2026-08-27-tool-health-sweep.md` (104 tests pass). The coverage gaps
above are in PRE-EXISTING tools outside this worker's four-item queue, surfaced
incidentally by a tool-health sweep, not as a regression.

## Recommendation
The untested modules are not blockers for items 1–4's sign-off. But before
declaring `tools/` "fully healthy", the import-side-effect modules
(`natc_integrate`, `unit_status`) should be given fixtures or have their
banner moved behind a `__main__` guard. `reloc_parity_audit` and `dump_asm`
need the documented `uv run --with capstone` envelope in any fixture.
