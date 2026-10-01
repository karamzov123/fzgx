# Tooling review packet — fleet/tool, 2026-08-27

Prepared by: **tooling worker (`tool`)**. This packet is for **hard-tail (`hard`)** sign-off.
Per the assignment rule: *no tool goes fleet-wide until it has a fixture test and the
hard-tail worker has reviewed it.* All four queue items are implemented, committed on
`fleet/tool`, fixture-tested, and live-verified. This file is the review artifact.

## Items (assignment order)

| # | Tool | Path | Status |
|---|------|------|--------|
| 1 | `find_xrefs.py --cslice` | `tools/find_xrefs.py` | done, committed, fixture-tested, live-verified |
| 2 | opcode-similarity index `similar.py` | `tools/similar.py` | done, committed, fixture-tested, live-verified |
| 3 | `emit_m2c_asm.py` (GNU-as PPC from target object) | `tools/emit_m2c_asm.py` | done, committed, fixture-tested, live-verified |
| 4 | `natc_loop.py` (state machine, LAST) | `tools/natc_loop.py` | done, committed, fixture-tested, live-verified |

## What each does (contract recap)

1. **`find_xrefs --cslice <sym>`** — dependency slicer. For a target symbol emits
   callers, callees, referenced globals with section/size/type, a relocation histogram,
   and a **minimal compilable header fragment containing ONLY the declarations that
   symbol's relocations require**. Cached by content hash under `~/.cache/natc/slice/`.
2. **`similar.py --symbol <fn> --k 5`** — normalises registers/immediates to classes,
   n-gram Jaccard over the 2,235 functions, returns the 5 nearest twins. No embedding
   model / GPU / download. `--accepted-only` restricts to functions from `accepted` units.
3. **`emit_m2c_asm.py <object> --symbol <sym> --m2c`** — GNU-as PPC for m2c from the
   **TARGET OBJECT** (relocations survive), preserving section directives, local/global
   labels, `R_PPC_*` as `@ha`/`@l`/`@sda21` operands, function boundaries. m2c source
   at `~/tools/m2c` (not the documented path).
4. **`natc_loop.py`** — orchestrates items 1–3 + `natc_compile.py`/`natc_feedback.py`
   per function. Context assembly is read-only, budget-independent (`--context-only`
   not gated by attempt budget).

## Fixture test results (this session)

| Suite | Result |
|-------|--------|
| `find_xrefs --self-test` | OK (victim=OSResetSystem, callers=3, frag lines=7) |
| `tests/test_find_xrefs.py` | OK (14 tests, full unittest) |
| `tests/test_emit_m2c_asm.py` | OK (14 tests) |
| `tests/test_natc_loop.py` | OK (18 tests) |
| `similar.py --self-test` (under `uv run --with capstone`) | OK (2235 fns; probe=GXSetMisc; 4 accepted-twin hits) |
| `tests/test_similar.py` (under `uv run --with capstone`) | OK |

Note: `similar.py` and `natc_loop.py` shell out to `uv run --with capstone` because
capstone is not in the base interpreter (PEP 668, no pip). This is by design — see
`tools/similar.py` / `tools/natc_loop.py` `run_similar()` / `run_m2c_seed()`.

## Live proofs (non-fixture, real DOL/objects)

### A. Per-unit object path — `OSInitAlloc` (main/dolphin/os/OSAllocHead)
`natc_loop.py --context-only --unit main/dolphin/os/OSAllocHead --symbol OSInitAlloc`
- Symbolised disasm: real relocations shown inline (`R_PPC_EMB_SDA21` per operand).
- XREF SUMMARY: 2 callers, 1 callee, 5 sda21 globals.
- MINIMAL HEADER FRAGMENT: 5 `extern int <sym>;  /* sdata/sbss scalar (T1) */` —
  correct scalar shape, **no `extern char x[4]` array miscast** (declaration-shape recipe).
- M2C SEED: driven from the **target object**, sda21 preserved (not rewritten to `@l`).
- SIMILAR TWINS + PROVENANCE SEED populated.

### B. DOL-only carve path — `fn_8006E250` (main/dolphin/mtx/MTX, fixed-coefficient ABI)
`natc_loop.py --context-only --unit main/dolphin/mtx/MTX --symbol fn_8006E250`
- Symbolised disasm: `lis r5, -0x2000` (base 0xE0000000) + `lfs f10,0xc(r5)`,
  `0x1c`, `0x2c` coefficient lanes.
- **TRAPS IN PLAY → T9: fixed-coefficient ABI** detected and surfaced (hard-tail's
  primary non-convertible barrier — do NOT model as SDK Mtx44/Vec).
- Xref/similar/m2c sections cleanly report `(unavailable: symbol not found in object
  index)` — documented DOL-fallback, **no crash**. Context assembly is crash-proof.

## Hard-tail-specific concerns addressed

- **sda21 not silently rewritten to `@l`** — covered by
  `test_emit_m2c_asm.test_emit_does_not_silently_rewrite_sda21_to_l` (passes).
- **Declaration-shape correctness** — `find_xrefs` emits T1 scalar shape; this is the
  exact recipe from the contract's "Declaration-shape recipe" section.
- **T8/T9 paired-single / fixed-coefficient traps** — detected in `natc_loop` symbolised
  disasm and surfaced in the TRAPS IN PLAY section (live-proven on fn_8006E250).
- **Crash-proof context assembly** — DOL-fallback and tool-unavailable paths return clean
  notes, never tracebacks (per `tests/test_natc_loop.py` `test_context_survives_broken_*`).

### C. Item 3 m2c wiring — full end-to-end (the one gap previously unproven)
`emit_m2c_asm.py --m2c` resolves m2c from the NON-documented path
`~/tools/m2c/m2c.py` (contract item 3) and invokes it as
`/usr/bin/python3 ~/tools/m2c/m2c.py -t ppc-mwcc-c <seed.s>`.
Live proof on a real object:
```
python3 tools/emit_m2c_asm.py build/GFZE01/obj/dolphin/os/OSAllocHead.o \
        --symbol OSInitAlloc --m2c --out /tmp/m2c_out.c   # RC=0
```
m2c returned real decompiled C:
```c
? OSAllocTableInit(s32, ?);                         /* extern */
extern s32 gAssetBudgetB;
extern s32 g_currentHeapHandle;
s32 OSInitAlloc(s32 arg0, s32 arg1, s32 arg2) { ... }
```
Conclusion: item 3's relocation-preserving assembler + m2c pipe (from the
correct, non-documented source) is fully wired and produces C end-to-end.

## Blocker before fleet-wide

Hard-tail sign-off required on items 1–4. After sign-off, the integrator (`integ`) is the
sole canonical `src/` writer and runs `natc_gate.py`. Tooling worker does NOT commit to
`main` and does NOT run `ninja`/the gate.

## Lease hygiene (this worker)

- `tool` holds **zero** conversion-unit leases (verified via `natc_rank.py --status
  --worker tool` and `SELECT worker='tool' FROM units` → `[]`).
- The stale `main/dolphin/dsp/DSP` lease held by `tool` was released as `deferred`
  on 2026-08-27 (its attempt was already reverted, commit d5b885e9), returning the unit
  to the pool for its real owner.
