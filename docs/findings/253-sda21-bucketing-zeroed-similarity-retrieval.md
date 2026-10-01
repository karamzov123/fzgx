# 253 — SDA21 displacement bucketing bug silently zeroed item-2 retrieval

Date: 2026-08-26
Worker: tool
Commit: c23e349 on `fleet/tool`

## Symptom

`tools/similar.py --symbol __DVDFSInit --k 5` printed its header and then
nothing. No error, exit 0. `--accepted-only` likewise empty. `--self-test`
passed, because its probes (`GXSetMisc`, `CARDRead`) are register/branch-heavy
rather than global-heavy.

## Root cause

`tools/psx_dis.py::normalise()` bucketed numeric operands with:

    if lit.startswith("0x") or lit.lstrip("-").isdigit():

Neither test matches `-0x7b18`: it does not start with `0x`, and sign-stripped
it is `0x7b18`, not a decimal digit string. So negative hex literals fell
through unbucketed and were emitted verbatim.

That form is not an edge case. It is the r13-relative SDA21 displacement MWCC
emits for **every** `sdata`/`sbss` global. `__DVDFSInit`'s four relocations are
all `R_PPC_EMB_SDA21` (`BootInfo`, `FstStart`, `FstStringStart`,
`MaxEntryNum`), so 12 of its 12 n-grams embedded an address literal unique to
that one function. Jaccard against all 2,234 others: 0.

The failure mode was the dangerous kind — the normaliser is supposed to remove
regalloc/addressing noise so similarity is invariant to it. Leaving one class of
addressing noise in made the affected functions *maximally dissimilar* rather
than noisy, and returned an empty list that reads like "no similar functions
exist" instead of "retrieval is broken".

## Fix

Sign-strip before both literal checks. `-0x7b18(r13)` now buckets to
`neg_big15(m)`, matching the existing `imm_big`/`neg_big` magnitude classes.

Confined to `normalise()`, reached only via `tokens()` -> `similar.py`.
`one()` and `rows()` are untouched, so `emit_m2c_asm.py` (item 3) still emits
operands verbatim and relocation fidelity is unaffected — checked, since a
normaliser leaking into the asm emitter would corrupt `@ha`/`@l`/`@sda21`.

## Verification

- `__DVDFSInit` now retrieves 5 neighbours (top `__AXPopCallbackStack` 0.062).
- Corpus reindexed: 2,235 fns in 0.7 s, one core, no model.
- New `NegativeHexBucketingTests` (3 cases) confirmed to FAIL on the old
  normaliser and pass on the fix.
- 11/11 fixtures green; `find_xrefs`/`emit_m2c_asm`/`natc_loop` self-tests rc=0.

## Second bug found while adding the guard

`tests/test_similar.py` had its `if __name__ == "__main__": unittest.main()`
guard **mid-file**, above two later class definitions. Appending a test class
ran 7 tests and printed OK while collecting none of the new ones. Guard moved to
EOF; count went 7 -> 10.

This was self-inflicted by appending to the file, not pre-existing. Swept all 11
fixtures afterwards: every `unittest.main()` now sits at EOF with zero class
definitions after it, so no other fixture is under-reporting. Recorded because
the failure is invisible — appending a class prints `OK` and a stale test count
rather than erroring. Append above the guard, and always check the ran-count
moved.

## Note on similarity scores

Top scores here are low in absolute terms (0.05–0.06) because these are tiny
functions — a 28–56 byte function has ~10–25 n-grams, so a couple of shared
grams is all the overlap available. Ranking is still meaningful; treat the
score as ordinal, not as a confidence. The `> 0.5` assertion in `--self-test`
holds only for genuine twins like `CARDRead`/`CARDWrite`.

## Non-tooling state preserved

Finding 252's uncommitted `src/` carve-generator regression was NOT present in
the tree this cycle (`git status` clean apart from my two tooling files). Nothing
discarded; no `reset`/`checkout`/`stash` used.
