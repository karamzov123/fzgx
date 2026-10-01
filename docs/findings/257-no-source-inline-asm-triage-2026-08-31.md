# No-source and inline-asm triage — 2026-08-31

This is an execution queue, not a candidate queue. Do not register or compile
an item here without a new evidence SHA and a current-head selector result.

## Verified current counts

- 210 historical 100%-scored records still contain asm in `src/`.
- 95 are compile-by-construction false positives.
- 98 require genuine reconversion.
- 112 have no source.
- 0 recovered artifacts are currently packageable.
- 178 remaining functions are known/expected to require inline asm for
  paired-single, supervisor, cache, or similar instruction forms.

## No-source protocol

For each no-source symbol, create one dossier only after a fresh action:

1. `python3 tools/natc_refs.py --dump --unit <unit>`
2. If a body exists, attach its path/provenance and reset the work to the
   reference-backed selector.
3. If no body exists, inspect retail disassembly, relocations, xrefs, strings,
   call sites, and field offsets.
4. Use a bounded read-only runtime trace only when the hypothesis concerns
   state, counts, ownership, or layout.
5. If no verified semantic fact results, mark blocked-evidence and release the
   unit; do not burn an attempt.

## Plateau protocol

A plateau unit is a sibling-group task. Identify every `<100%` sibling from the
whole-unit report, then work the smallest group that can raise the whole unit
to 100%. Rebase from canonical `main` after every accepted sibling. Never emit
parallel sibling candidates from one stale snapshot.

## Inline-asm completion tranche

For each irreducible instruction family:

- preserve the exact instruction sequence in an explicitly bounded inline-asm
  block;
- convert surrounding control/data flow to natural C where compiler parity is
  achievable;
- retain retail symbol names and relocations;
- validate full DOL and runtime semantics;
- count it as complete-source progress, not failed natural-C progress.

This tranche must not weaken the natural-C metric or replace gate evidence.

## Current blockers

- No ready batch exists.
- The 98 reconversion items need genuinely changed context.
- The 112 no-source items need evidence, not model retries.
- Whole-unit gate plateaus need sibling-pair/group landings.
