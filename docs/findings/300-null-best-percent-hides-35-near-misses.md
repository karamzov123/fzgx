# 300 -- 613 unmatched functions have NULL best_percent, and 35 of them are near-misses

# `functions.best_percent` is what every ranking in the fleet sorts on: `choose()` orders
# candidates by `-best_percent`, `size_allowed()` gates the size band on it,
# `unsearched_near_misses()` selects on `best >= 90`, and `one_word_tier()` is regenerated from
# the same band. A NULL reads as "no score", which sorts below every function that has one.
#
# Measured: **613 of 1550 unmatched functions have NULL best_percent**, yet 613 of them also have
# attempt rows carrying a real `final_percent`. The column is not being written back when an
# attempt ends, so a function's best measured score exists in `attempts` and nowhere the fleet can
# see it.
#
# ## 35 of them are near-misses
#
# Joining `attempts` for the max score per symbol, 35 of the 613 are at or above 90%:
#
#     fn_1_17098       main_rel   99.469   (reproduces at 98.168%, 10 differing words)
#     fn_1_9FA18       main_rel    98.943   (reproduces at 95.699%, 12 differing words)
#     sample:_prolog   sample      98.050   (body no longer reproduces: 41.4%)
#     fn_1_93734       main_rel    97.528
#     fn_8004CD70      main        97.008
#     fn_1_A1DCC       main_rel    96.700
#     fn_1_10C4E4      main_rel    96.139
#     ... 35 total
#
# `fn_1_17098` is the clearest case: an archived body that scores 98.168% with 10 differing words,
# absent from the one-word tier, absent from every `-best_percent` ordering, and therefore never
# selected. It has one attempt, which is what kept it under the attempt cap, so the fleet was
# willing to work it -- it just never saw it.
#
# Carved here: fn_1_17098, fn_1_9FA18, fn_8004CD70, fn_1_A1DCC. `sample:sample:_prolog` is not
# resolvable by that name (`carve` rejects it as ambiguous) and its body no longer reproduces, so
# it is left alone.
#
# ## Why this is not just cosmetic
#
# This is the same class of defect as the stale one-word tier (finding 299's predecessor): a
# measurement the project made that nothing consumes, so work the fleet already paid for is
# invisible. The fix is a backfill -- `UPDATE functions SET best_percent = (SELECT
# MAX(final_percent) ...)` -- but that writes the ledger, and the ledger is the project's record of
# what was verified, so it should not be patched from a session. Recorded instead, with the query
# that produces the numbers.
#
# A cheaper safe variant exists and is worth stating: `one_word_tier()` regenerates its own file
# from the saved bodies rather than from `best_percent`, so it is not affected by this bug. The
# exposure is confined to `choose()`, `size_allowed()` and `unsearched_near_misses()`.
