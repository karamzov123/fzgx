# 102: sda21 asm-spike RE-CONFIRMED NEGATIVE — the A-2 frontier needs a different lever

Date: 2026-08-24 ~16:30 EDT. Follows up on the fleet audit
(~/pm-supervisor/AUDIT-FLEET-2026-08-24.md §1.3, FINDING A-2), which proposed
that relocation symbolisation (`lwz r5, gx@sda21`) was a mechanically
automatable path from 39% → ~85% matched_code, gated only on "does MWCC's asm
block accept a symbolic D-form operand".

## Spike result: MWCC 1.2.5n inline asm CANNOT emit sym@sda21

Tested on `src/dolphin/gx/GXAttr.c` :: `__GXXfVtxSpecs` (the audit's own
example site), compiler GC/1.2.5n via wibo:

| Source form | Compiler verdict |
|---|---|
| `lwz r5, gx@sda21` | `Error: end of line expected` |
| `lwz r5, gx@sda21(r0)` | `Error: end of line expected` |
| `lwz r5, sda21(gx)` | `Error: gx was not assigned to a register (try using register qualifier)` — and `register GXData *const gx;` extern is `illegal storage class` |

This re-confirms findings/06 ("mwcceppc inline asm REJECTS `sym@sda21`") and
findings/12 note 7 (hardcoded `-0x7DE8(r2)` literals are the accepted
workaround). The audit's caveat was warranted: **the 39%→85% ceiling via asm
symbolisation does not exist as a mechanical sweep.**

What DOES work (already proven in-repo):
- Natural C data access emits `R_PPC_EMB_SDA21` relocs correctly
  (findings/06 __OSGetInterruptHandler).
- `sym@ha` / `sym@l` ARE accepted in inline asm (PROGRESS.md tooling win,
  OSException.c) → ADDR16_HA/LO parity is still mechanically reachable.
- `sda21(gx)` with a register-classed symbol may work under different decl
  forms (`asm register` keyword combos) — one more spike variant worth a
  single attempt before closing.

## Revised decomp strategy

1. **ADDR16_HA/LO sweep** (837 sites / ~11% of diffs): mechanical,
   syntax proven in-repo. Free-agent fan-out.
2. **GLOBAL-NAMED conversions** (185 funcs / 87,808 B): run
   `tools/convert_global_named.py` per unit. Mechanical. Free fan-out.
3. **Orphan unit registration** (GXInit + 5 others = 9,584 B): free, ~1 h.
4. **Natural-C conversion of small-data-heavy functions** instead of asm
   edits for the 90–99% band: where a function is pure enough that natural C
   reproduces it (as OSInterrupts proved), the compiler emits sda21 relocs
   for us. This is slower than a text sweep but is the only route to those
   bytes. Paid/luna review of each candidate unit.
5. RENAME conveyor demoted to free background stream (unchanged from audit).

Metric honesty: objdiff pre-link scoring means the linked DOL stays EXACT
through all of this; the sha1 gate remains the safety invariant.
