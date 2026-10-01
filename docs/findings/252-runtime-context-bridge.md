# 252 — Runtime evidence now reaches NATC worker context

Date: 2026-08-30
Scope: tooling/context only; no source candidate, lease, queue, compiler pin, or Dolphin input was changed

## Problem recovered

The F-Zero Online side already had a bounded read-only trace emitter, deterministic input-manifest support, virtual-pad replay validation, and an offline runtime comparator. The decomp side already had a fail-closed `natc.trace.v1` importer. The chain stopped there: `tools/natc_loop.py --context-only` assembled disassembly, relocations, xrefs, references, m2c, twins, provenance, and traps, but did not consume imported runtime evidence.

A pre-change probe against `main/dolphin/card/CARDBlock::__CARDGetControlBlockReady` returned no `VERIFIED RUNTIME FACTS` section even though that function references `__CARDBlock` and a matching live trace artifact existed. This made the emulator/layout work invisible to conversion workers and encouraged repeated ABI/layout guesses.

## Implemented bridge

`tools/natc_runtime_import.py::runtime_context_section()` now:

- walks every runtime-cache path component from `/` with `openat(..., O_DIRECTORY|O_NOFOLLOW)`, enumerates no more than 128 entries from the final held descriptor, and opens no more than 32 `.jsonl` bundle members relative to it; each trace, manifest, and marker is captured once through a bounded `O_NOFOLLOW` regular-file descriptor, so intermediate/final directory symlinks, member symlink swaps, growth beyond its ceiling, and path re-open races fail closed;
- revalidates the artifact, manifest, completion marker, `GFZE01`, exact retail DOL SHA-1, exact retail map ID, live emulator identity, read plan, and run consistency through `import_trace()`;
- selects observations only when the trace symbol token matches the target function or a global reported by `find_xrefs.py`;
- emits no more than 16 facts;
- JSON-escapes all trace strings before prompt insertion;
- renders observations up to 8 bytes as fixed-width hex and larger observations only as SHA-256;
- contributes no facts from incomplete, invalid, wrong-DOL, or unrelated bundles.

`tools/natc_loop.py::build_context()` collects the target symbol plus `globals_referenced` and appends the evidence-only section. Runtime evidence does not affect attempt budgets, source, scoring, queue transitions, or acceptance gates.

## Legacy completion-marker repair

The two 2026-08-29 artifacts and manifests were unchanged and their manifest-declared artifact SHA-256 values matched their current bytes, but they predated the hardened requirement for adjacent `natc.trace.complete.v1` markers. The current importer therefore rejected both before this repair.

Only completion metadata was added; trace bytes, manifests, identities, and observations were not modified:

- `~/.cache/natc/runtime/layout-audit.jsonl.complete.json`
  - trace SHA-256: `8886dbfbf36fb9594e745887de42a1dbc339dd01bcaac138e52eb58ed0e1a895`
  - marker file SHA-256: `4bf246742386e973d25bbec938177f4646bdac5e176f569a09194820f75b0bbe`
- `~/.cache/natc/runtime/layout-deref.jsonl.complete.json`
  - trace SHA-256: `cc44a2d5bd75b3ab1fbcff79f8eecda50a9f054b7ec5bbdffd03770eab9c110f`
  - marker file SHA-256: `0063484665223970ee7012bdcd380a53f597488e0c6464379c540fd12cbaf826`

Both bundles then passed the current `import_trace()` implementation: 4 records for `natc-layout-audit-20260829` and 5 records for `natc-layout-deref-20260829`.

## Real context proof

Command:

```
python3 tools/natc_loop.py \
  --unit main/dolphin/card/CARDBlock \
  --symbol __CARDGetControlBlockReady \
  --context-only
```

The command returned zero and selected the verified global observation:

```
symbol="CARD.__CARDBlock[2]" address=0x80177960 width=544
bytes_sha256=6bc6fbf18d69ff2ecf6715613d37d558d903fb9e0a1e5b32decf0e62d94fa537
path_facts=["two 0x110-byte CARD control blocks; compare channel stride and pointer fields"]
```

Its provenance remains attached: DOL `421c88106697d3275a3fc26fb7a01bf6d816b271`, map `map-sha256:b74a0d117ed93d1db4babb2a8ca6f3e8c6de3e0058d1594e75358eac71fbbf58`, repository revision `e0f9f5d0f68757df6b7028ac4ea85314e25c8c5c`, and the exact artifact SHA-256 above.

## Verification

- `uv run --extra test pytest -q` — 272 passed
- `python3 -m py_compile tools/natc_runtime_import.py tools/natc_loop.py tests/test_natc_runtime_import.py tests/test_natc_loop.py` — passed
- `git diff --check` — passed
- real `CARDBlock::__CARDGetControlBlockReady --context-only` probe — returned zero and selected the expected artifact, map, symbol, width, and path fact
- added-line security scan — no hardcoded secret, `shell=True`/`os.system`, `eval`/`exec`, or pickle-load pattern

The unsupported raw `unittest discover` invocation was also tried and failed for pre-existing environment/harness reasons (`pytest` and `capstone` absent from system Python, plus queue tests that require their pytest fixtures). It was replaced by the repository-declared `uv run --extra test pytest` command above; no failing result was hidden or reclassified as a product pass.

## Next evidence actions

1. Capture dynamic PAD/controller transition traces with a bounded `natc.input.v1` manifest. The existing boot capture proves `__PADSpec == 0` only for that state; it does not prove all per-channel offsets.
2. Do not run retail-vs-candidate replay until the candidate DOL, map, ISO/profile, and embedded identities are demonstrably distinct from retail.
3. Add the separately proposed runtime-evaluation ledger link (`candidate_id` + `hypothesis_id` + both trace hashes + input-manifest hash) before using a runtime comparison as durable candidate evidence.
4. Keep runtime facts evidence-only. MWCC/object/relocation/full-DOL gates remain authoritative.
