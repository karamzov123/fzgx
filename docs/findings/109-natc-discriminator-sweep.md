# findings/109 — compiler discriminator sweep, 2026-08-25 (integrator)

Method: for every objdiff unit with ZERO exact natural-C functions, compile a
known-good candidate body with `tools/natc_compile.py --discriminate`
(GC/1.2.5, 1.2.5n, 1.3, 1.3.2, 1.3.2r) and diff each object against the
target. Lowest-diff version wins; ties broken by prologue class
(A-family stwu-first ⇒ GC/1.3 family per T11).

## Results

| unit | old pin | winner | evidence |
|---|---|---|---|
| game/tail_800410A4 | 1.2.5n | **GC/1.3** | 12/13 fns EXACT vs 0/13; second confirmed mispin after lightctrl |
| dolphin/mtx/MTXHead | 1.2.5n | GC/1.3 | A-family stwu-first retail prologue |
| game/model_80072EDC | 1.2.5n | GC/1.3 | same |
| game/model_80074D88 | 1.2.5n | GC/1.3 | same |
| dolphin/os/OSAllocHead | 1.2.5n | GC/1.3 | same |

All re-pins applied PER-UNIT in configure.py (`mw_version="GC/1.3"`).
The global compiler default is NOT flattened (T19 discipline).

## Rule going forward

Before attempt 6 on any zero-exact unit: run the discriminator. Five compiles
= ~125 ms. A pin mismatch is indistinguishable from a hard function and costs
up to 28 wasted attempts.
