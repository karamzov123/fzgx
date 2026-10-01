# natc_loop --candidate attempt-flow wiring — 2026-08-27 (fleet/tool)

New tool-health probe this cycle: does the conversion attempt path (the part
of `natc_loop.py` that actually drives a conversion via the four authored
tools + `natc_compile`) have a sound, reachable compile dependency? Earlier
cycles proved `--context-only` (×2) but never the real `--candidate` compile.

## What `--candidate` does (from natc_loop.py main)
- Requires `--worker` (attribution to a real worker ledger; prevents poison).
- Gated by `BUDGET` (12 attempts); T19 discriminate guard at >=4.
- Calls `run_attempt()` -> `natc_compile.py --unit --src --symbol --worker`,
  which is the ONLY component allowed to invoke MWCC (NATC-STRATEGY §4.1).

## Action taken (non-side-effecting — no lease, no ledger write)
1. Confirmed `build/compilers` is a **symlink** to the canonical repo
   compilers; `mwcceppc.exe` present + reachable:
   `build/compilers/GC/1.2.5n/mwcceppc.exe` (1.6 MB, Aug 27 14:26).
   (Earlier cycle wrongly inferred "compilers absent" from a `ls build/tools/`
   listing that does not include the separate `compilers/` dir — corrected.)
2. Ran `natc_compile --self-test` (the in-process lock for canonical-command
   assembly, Finding 259 — deliberately does NOT shell out to mwcc):
   `[natc_compile] SELF-TEST OK (canon edge found; -c/-o assembly correct
   for main/dolphin/os/OSError)`  -> rc=0.
3. Confirmed the attempt-flow admission guard is test-locked:
   `tests/test_natc_compile_admission.py::test_exact_candidate_context_is_blocked`
   PASSES (exact-candidate replay is refused — prevents budget-dodging).

## Why a live MWCC compile+score was NOT run here
- A real compile writes to the attempt ledger (`RUNS_DB`) and is gated by a
  worker lease + `BUDGET` (contract L108-110). Running it would either (a)
  require claiming a unit lease (forbidden for the tooling worker — it hides
  the unit from its real owner) or (b) poison a worker's ledger / spend an
  attempt. Both are barred (L13/L108-110/L143).
- Therefore the live compile is an **integration/conversion-worker domain**
  action, correctly out of tool-worker scope.

## Conclusion
The `--candidate` attempt path is wired correctly and its compile dependency
is sound + reachable:
- context assembly: proven live (OSInitAlloc full path; fn_8006E250 DOL-fallback+T9).
- `natc_compile`: self-test OK (canonical flags locked), admission guard OK,
  MWCC binary reachable via symlinked `build/compilers`.
- build tree: present, byte-identical to canonical, natc_metrics runnable.
The only un-run step is the actual mwcc score, which belongs to a conversion
worker under lease — not the tooling worker.

## Final tool-worker status
All four queue items: built, fixture-tested, live-verified (context), promoted
to main, hard-tail ratified (77aba4a from fleet/hard). Tool-health sweep
complete across every dimension: suite (112 pass), CLI self-tests (7/7),
harvest (3 pkg-able staged), health-state decode (fleet gauge), build
integrity (matches canonical), and attempt-flow compile dependency (sound +
reachable, live score out-of-scope). No further tool-worker action exists.
