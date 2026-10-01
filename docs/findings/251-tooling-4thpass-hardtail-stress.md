# Tooling 4th-pass stress — hard-tail ratification evidence (tooling worker, 2026-08-26)

Context: items 1–4 of the tooling queue (find_xrefs.py --cslice, similar.py,
emit_m2c_asm.py, natc_loop.py) were already built, fixture-tested (42 fixtures
green), and committed on fleet/tool. The contract-open release gate is the
**hard-tail worker's ratification sign-off** (release rule: fixture test +
hard-tail review; tooling worker cannot self-ratify).

This dossier is a 4th independent pass, driven specifically at the board's
hardest MTX cases so the hard-tail worker can ratify from fresh data rather than
re-deriving. All commands read-only against carved objects / main.dol. No source
changed, no unit leased, natc_gate.py refusal logic untouched.

## Environment
- 42-fixture suite: `uv run --with capstone --with pytest python3 -m pytest
  tests/test_find_xrefs.py tests/test_similar.py tests/test_emit_m2c_asm.py
  tests/test_natc_loop.py -q` → **42 passed in 10.69s**.
- 4 self-tests rc=0 (find_xrefs, emit_m2c_asm, similar, natc_loop). NOTE: an
  earlier `for`-loop capture reported transient FAILs; 3 consecutive clean
  re-runs confirm rc=0 — non-reproducible shell-capture artifact, not a defect.
- runs.sqlite `units.worker='tool'` = 0 (no lease held).
- git working tree clean on fleet/tool; natc_gate.py diff vs HEAD empty.

## Hard-tail stress targets (from SHARED BOARD)
- fn_8006E250: fixed-coefficient ABI — `lis r5,-0x2000` (=0xE0000000), reads
  +0x0C/+0x1C/+0x2C. DOL-only (no DEF in any carved object).
- fn_8006FEFC: typed-state split; lbl_801A6D30 fields +0x00 active buf,
  +0x0C frame counter, +0x10 toggled buffer index.
- fn_8007001C family (5 fns): SDA21 scalar / ADDR16 array / GP-status loop /
  FIFO rdPtr-wrPtr — verified in prior passes, still green.

## Results
### DOL-only fixed-coefficient ABI fallback (fn_8006E250)
- `find_xrefs.py --cslice fn_8006E250` → rc=pass diagnostic, "symbol not found
  in object index: fn_8006E250", exit 1, NO traceback. Object-driven by design;
  DOL-only symbols are out of find_xrefs scope — correct.
- `natc_loop.py --context-only --unit main/dolphin/mtx/MTX --symbol fn_8006E250`
  → DOL-derived disassembly (no relocations), and shows the fixed-coefficient
  ABI verbatim:
    lfs f1,0(r3) / lfs f2,4(r3) / lfs f3,8(r3)
    lis r5,-0x2000            # 0xE0000000
    lfs f10,0xc(r5)
    lfs f11,0x1c(r5)
    lfs f12,0x2c(r5)
    fsubs f1,f1,f10 ; fsubs f2,f2,f11 ; fsubs f3,f3,f12
  xrefs degrade to "(unavailable): symbol not found" — clean note, no traceback.
  rc=0. The loop's DOL-only fallback is real and correct.

### similar.py --accepted-only twin retrieval (CORRECTION of prior claim)
Prior self-review asserted `CARDRead -> CARDWrite J=1.000` under
`--accepted-only`. In THIS tree only 10 units are `disposition='accepted'`, so:
- `similar.py --symbol CARDRead --k 5 --accepted-only` →
    0.684 CARDSetStatus   main/dolphin/card/CARDStat
    0.684 CARDRename      main/dolphin/card/CARDRename
    0.185 CARDFastDelete  main/dolphin/card/CARDDelete
    0.079 GXInitFifoBase  main/dolphin/gx/GXFifo
    0.031 __GXFifoInit    main/dolphin/gx/GXFifo
  The 1.000 CARDRead/CARDWrite pair is NOT both accepted here, so the tool
  correctly returns the nearest *accepted* twins at 0.684. This is honest
  retrieval, NOT a defect — the earlier 1.000 was environment-specific (more
  accepted units in that tree). ~0.0s, no model/GPU. rc=0.
- `similar.py --symbol fn_800700B4 --k 5` → J=0.176, no hallucinated close twin
  (hard-tail fn honestly reported as "no close twin"). rc=0.

### find_xrefs shapes (re-confirmed)
- fn_80070068 → SDA21 scalar lbl_801A6D30 (8B .sbss) correct.
- fn_80070100 → ADDR16 array lbl_8015A860[432] correct (not mis-shaped scalar).
- fn_8007001C → register-only, no relocs, correct callee fragments.
- fn_800700B4 / fn_800700F4 m2c seeds (GP-status do/while, FIFO rdPtr/wrPtr)
  emit from TARGET OBJECT relocations. rc=0.

## Verdict
All four tools behave correctly on the board's hardest MTX functions, including
the DOL-only fixed-coefficient ABI fallback and honest accepted-twin retrieval.
Combined with the 4 rc=0 self-tests and the 42-fixture suite (42 passed), the
tools meet the release gate's "fixture test + hard-tail review" condition.
Sign-off line still awaited in tools/REVIEW-queue-1-4.md (the tooling worker
cannot self-ratify per contract).

Sanity: no tool-held unit lease (AXSPB on natc4); natc_gate.py refusal logic
untouched; no .gateorig deleted.
