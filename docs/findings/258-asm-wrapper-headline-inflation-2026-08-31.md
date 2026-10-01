# The mission headline counted 26 functions that contain inline asm — 15 of them containing no C at all

2026-08-31. Found while opening the inline-asm completion tranche (finding
257, item 6 of the current work order).

## What was wrong

`natc_metrics.collect()` decided "is this function still assembly?" with
`asm_symbols()`, which matches an `asm void f()` whole-function definition.
There is a second way to write a function entirely in assembly that it never
saw:

```c
void UnsetRun(register void* thread)
{
    asm
    {
    lwz     r4, 0x2e0(r3)
    ...
    blr
    }
}
```

MWCC compiles that to the retail bytes. objdiff scores it 100%. The symbol
appears in our object, so it is in `ours`. It is not in `asm_names`. Every
branch of the classifier therefore fell through to:

```python
elif pct >= 100.0 and name in ours:
    exact_c.append(...)          # counted as an EXACT NATURAL-C CONVERSION
```

The only thing standing between this shape and the headline was
`natc_gate.inline_asm_c_defs()`, which refuses such a candidate at submission
time. That check post-dates the 26 functions already at HEAD, and a check in
the gate cannot un-count what is already in `src/`.

## Verified counts at HEAD

`tools/natc_asmforms.py` classifies the three forms. All 26 were inside the
published 347:

| form | count | verdict |
|---|---|---|
| asm **wrapper** — C signature, body is one `asm { }` block, no C | 15 | not a conversion; belongs in the remaining denominator |
| **hybrid** — real C around a bounded `asm { }` statement | 11 | complete source, genuine progress, but not natural C |

Wrappers: `fn_8008CB20/28/30/38` (metrotrk/init), `EXIClearInterrupts`,
`DCEnable`, `ICFlashInvalidate`, `ICEnable`, `LCLoadBlocks`, `LCStoreBlocks`,
`__OSModuleInit`, `Callback`, `OSSetSaveRegion`, `OSGetSaveRegion`,
`UnsetRun`.

Hybrids: `OSInitArenaPoll`, `OSAllocFromArenaLo`, `L2GlobalInvalidate`,
`OSGetStackPointer`, `OSDisableInterrupts`, `OSEnableInterrupts`,
`OSRestoreInterrupts`, `OSGetPhysicalMemSize`,
`OSGetConsoleSimulatedMemSize`, `__OSGetDIConfig`,
`__OSGetEffectivePriority`.

## Effect on the numbers

```
before   347 exact natural-C / 22,892 B   15.570% exact   1862 asm remaining
after    321 exact natural-C / 22,476 B   14.362% exact   1877 asm remaining
         + 11 hybrid / 416 B reported in its own additive lane (2b)
```

The DOL is unchanged and still green (`421c8810…`). Nothing regressed; the
measurement was wrong and now is not. 321 is the honest pure-natural-C count.

## Why this had to land before the tranche, not after

Finding 257 opens a lane that deliberately writes bounded inline asm inside C
functions. Had the gate been relaxed for that lane first, every function the
tranche landed would have flowed straight into the natural-C headline — the
tranche would have manufactured mission credit at roughly one function per
landing, and the failure would have looked exactly like success. The
classifier is the precondition for the lane, not a cleanup after it.

This is the same shape as the earlier false-metric defects (the score that
returned 100 for an unmodified asm body; `elf_func_bytes` counting data
symbols as functions): a measurement that credits work nobody did.

## What is now in place

- `tools/natc_asmforms.py` — `classify_inline_asm(text) -> (wrappers, hybrids)`,
  plus a CLI that audits `src/`. Conservative by construction: a function is
  only called a hybrid when C survives after every asm block is removed, and
  signature text does not count as C.
- `natc_metrics` counts wrappers as asm-bodied, puts hybrids in lane `2b`, and
  emits `inline_asm_hybrid_functions` / `asm_wrapper_functions` in `--json`
  and in the ledger the desktop bar and supervisor read.
- `tests/test_natc_asmforms.py` — pins the three forms apart, pins the
  `asm void f()` body out of the classifier so it is not double-counted, and
  pins the 15/11 HEAD census so a future move must be deliberate.

## Still open

- `natc_gate` keeps its own copy of the same regexes (`asm_defs`,
  `c_function_ranges`, `inline_asm_c_defs`). They agree today; they are two
  copies and will drift. The gate should import `natc_asmforms`. Not done in
  this change because the gate is load-bearing and the fleet was mid-flight.
- The gate still refuses ALL inline asm in a candidate. The tranche needs a
  per-symbol enrolment allowlist there — a blanket relaxation would re-open
  the wrapper path into `src/`.
- The 15 wrappers are now correctly counted as remaining work. They are
  legitimate tranche targets: each is a real function whose surrounding
  control flow may be expressible in C even where the core instruction is not.

---

## Follow-up, same session: the tranche lane is now open

The three blockers listed above under "Still open" are resolved except the
regex duplication.

**Gate.** `source_form_check` no longer refuses inline asm outright. It now
calls `inline_asm_tranche_errors(candidate, destination, card)`, which admits
a candidate only when all three hold:

1. it is a **hybrid**, not a wrapper — real C survives once the asm blocks are
   removed;
2. the **destination's** retail asm body for that symbol contains a
   paired-single / supervisor / cache instruction. The evidence is read from
   the canonical tree, never from the candidate, so a candidate cannot enrol
   itself, and a card naming a symbol whose retail body is ordinary integer
   work does not admit it;
3. `CARD.md` declares `inline-asm: <symbol> — <class and why>`, so an asm
   block a worker forgot to remove cannot pass as a deliberate conversion.

**Selector.** `natc_symbol_state.py --next --tranche` returns *only* symbols
already classified `inline_asm_required`, with their `natc_eligibility`
evidence and the exact CARD line to add. Without `--tranche` those symbols
stay excluded exactly as before, so the natural-C lane is unchanged — a worker
asking for natural-C work is still never handed a function C cannot express.

Measured unlock:

```
units carrying inline_asm_required symbols                 35
units with tranche work available                          33
  of those, units with NO natural-C work left at all       18   <- was dead inventory
tranche symbols now reachable                             147
```

Eighteen units had every symbol blocked: `--next` returned
"no fresh-budget natural-C sibling" and nothing in the fleet could ever reach
them again. `main/dolphin/os/OSCache` is typical — natural-C lane empty,
tranche lane offers `DCFlushRange` with evidence
`natc_eligibility:cache/sync,supervisor`.

Suite 338 green. DOL unchanged and green.

## Still open after this

- `natc_gate` still carries its own `asm_defs` / `c_function_ranges` /
  `inline_asm_c_defs`, duplicating `natc_asmforms`. They agree today. The gate
  should import the module; deferred because the gate was live.
- No worker prompt drives the tranche yet. Assigning it needs an edit to
  `~/pm-supervisor/natc/worker-<w>.md` — the GENERATED
  `~/.cache/natc/prompts/<w>.txt` is rewritten on every launch and an edit
  there is silently reverted (supervisor-v5.sh:125).
- The 15 wrappers already in `src/` are now correctly counted as remaining.
  They are tranche targets: converting one means writing the surrounding
  control flow as C and keeping only the irreducible instruction in the block.
