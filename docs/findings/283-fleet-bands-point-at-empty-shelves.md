# 283 — the fleet's size bands pointed at empty shelves: 57% of the budget on a band measured at 0

Date: 2026-10-04. Tool: `tools/fleet_multi.py`, `choose()`, `FAMILY_BANDS`.

## The measurement that started it

Attempts over the preceding six hours, bucketed by each function's current `best_percent`:

| band | attempts | share |
|---|---:|---:|
| **>=99%** | **56** | **57.1%** |
| 95-99 | 16 | 16.3% |
| 85-95 | 12 | 12.2% |
| <85 | 11 | 11.2% |
| cold (never scored) | 3 | 3.1% |

Finding 282 had just measured the ≥99% band at **0 matches from 40,208 candidates**. So more
than half the fleet's budget was going where the deterministic engine had just proven nothing
converts, and 3% was going to the 549 untouched functions. Attempts per match was 13.6 and
rising; this is where that came from.

## The cause was not the sort

The obvious reading is a selection bug: `choose()` orders virgin-first and the near-miss set
arrives as `retry=`, so one might expect a ranking fix. **That reading is wrong, and the
measurement says so.** Virgin supply by size band, against the real backlog:

| band | backlog | virgin | attempted |
|---|---:|---:|---:|
| <=256 | 198 | 4 | 194 |
| 257-512 | 283 | **0** | 283 |
| 513-1K | 514 | **0** | 514 |
| 1-2K | 399 | **339** | 60 |
| >2K | 214 | **206** | 8 |

`cline` was banded to (257, 512) and `oc1` to (513, 1024). Those two bands contain **zero**
virgin functions between them — every function in them has already been attempted. Their
virgin supply was not out-ranked, it was *absent*, so both families were structurally forced
into re-deriving saved near-misses. Only `oc4` and `gpt`, on wide bands, could see the 545
virgin functions sitting above 1 KB.

The bands were sized when the small pools were fresh; they were never revisited as those pools
drained. A band is a supply statement, and it went stale silently — nothing errors when a band
empties, because the widening path in `choose()` falls back to the whole backlog and the family
keeps producing plausible-looking batches.

Verified directly against the live backlog, `choose(..., retry=near_misses)` per family:

    before:  cline 0/6 virgin   oc1 0/6 virgin   oc4 6/6   gpt 6/6
    after:   cline 6/6 virgin   oc1 1/6 virgin   oc4 6/6   gpt 6/6

cline and oc1 went from structurally starved to supplied. oc1's 1/6 is correct rather than a
regression: `SIZE_GATE=2048` admits a function above 2048 B only as a saved near-miss at
>=95%, so most of its virgin band is not schedulable and the widening path supplies the rest.

## Two things tried first and reverted

Recorded because they are the natural next thing to try, and because reverting them is the
finding:

1. **A fresh-work quota in `choose()`** (reserve 75% of each batch for virgin symbols). It
   changed nothing: with an empty virgin pool in a family's band there is nothing to reserve
   *for*. Correct mechanism, wrong level.
2. **Reordering the FAMILY_LOCKOUT sort to keep virgin-ness as the leading key.** Also changed
   nothing, for the same reason — and it would have discarded the existing `order` tuple.

Both were reverted. The fix is one table.

## The change

    cline: (257, 512)   -> (1024, 2048)    339 virgin, 12.1% measured conversion
    oc1:   (513, 1024)  -> (2048, 1<<30)   206 virgin, mostly SIZE_GATE-gated

The conversion figures are the ledger's own, per size band: 1-2K converts at 12.1% (55/454),
512-1K at 39.3% (333/847) but with 0 virgin left, >2K at 7.0%. cline is the family with the
highest measured yield per session, so it gets the largest supplied pool.

## Regression check

With the virgin pool removed entirely, every family still fills 6/6. A starved family must
degrade to near-misses rather than idle, and that behaviour is unchanged.

## What to watch

Re-run the virgin-by-band table before touching a band. The failure mode is silent: an
exhausted band does not raise, it just quietly converts a productive family into a
re-derivation loop, and the only visible symptom is a yield number weeks later.