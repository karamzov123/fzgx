# 282 — the ≥99% band does not yield to deterministic repair: 40,208 candidates, 0 matches

Date: 2026-10-04. Tool: `fzgx sweep --min-percent 99 --rounds 3 --land`.

## What this tests

Move 5 of the completion plan proposed building a deterministic search driver on the idle
cores to attack the hard tail, on the reasoning that the substrate already exists (spill-graph
capture/replay, level probing, declaration projection) and only the search loop is missing.
The plan timeboxed it: *"if it hasn't produced matches in 3 weeks, stop."*

That premise deserved a measurement before three weeks of building, because
`docs/REGISTER_REPAIR.md` already records the honest signal: **"No new C matches resulted"**
from captured declaration-order projection, across 170 functions and 259 allocation passes.

## The measurement

`fzgx sweep` is the existing fast path of the repair engine -- every cheap family (evidence,
source and declaration moves) over a whole selection, one batched compile per module per round.
On the entire ≥99% unmatched population:

    functions 135
    round 0: 135 functions, 13359 candidates
    round 1: 135 functions, 13432 candidates
    round 2: 135 functions, 13417 candidates
    matched 0 []
    improved 1
      fn_1_10A6A0   99.04 -> 99.43  ['decl: move p2 to 3', 'decl: move pool to 0']
    submitted 0 []
    saved 1
    failed 0 []

**40,208 candidates across three rounds. Zero matches.** One body improved, by 0.39 points.

## What this means

The ≥99% band is not waiting for a better search. It is 135 functions whose remaining
difference is not reachable by any move the repair engine knows how to make, and the engine
can enumerate 13,000 candidates per function per round without closing one of them.

This is consistent with the rest of the measured picture rather than surprising next to it:

- `declaration_projection`'s own funnel is 361 → 26 → 7 → ~4 (finding, 2026-10-03).
- The `lintallow` pass converted 1 in 7 and the class is exhausted; 304 of 305 need real
  source changes, not a mechanical pass.
- A 99.5–100% plateau exit converts only **18.8%** of the time *given unlimited further
  attempts* (finding 274), so this band is structurally stuck rather than under-searched.
- Register repair: 0 matches from 170 functions of captured allocator decisions.

Four independent lines now agree. **The hard tail is a source-shape problem, and the
deterministic engine cannot solve it.** Move 5 is retired, not deferred: building the driver
would have spent three weeks to produce the 0 this run produced in nine minutes.

## What is actually left for this population

The remaining difference in these 135 is something a compiler-visible source change has to
express, and finding the change is the same class of work the fleet already does: read the
diff, find the structural cause, repair the source. The honest owner for it is a model
session, not a search.

Note what this does **not** say: it does not say the fleet should keep spending on this band
either. `sweep --min-percent 99` is the cheap way to establish that the mechanical lever is
exhausted here, and it costs nine minutes. Re-running it after a fleet change is a valid
regression check; building more of it is not the answer.

## Side effect, recorded because it is the kind of thing that gets lost

`--land` saved `fn_1_10A6A0` at 99.43% (up from 99.04) as an attempt, with `decl: move p2 to 3`
and `decl: move pool to 0` in its history. That is the engine working correctly -- it keeps the
best body even when it cannot close the function. The tree stayed clean; no source, split or
units.json change was made, and `16 files OK` is unaffected.

## Cost

Nine minutes wall, no commits, no config touched. Cheapest possible answer to a three-week
proposal.