# 294 -- the size gate was closed on eight samples; measured as a probe instead of a decision

#
# Finding 293 left `SIZE_GATE` alone and recommended re-measuring it. Done. The gate did not
# move, but the reason it stands is now honest and the thing it was costing is now known.
#
# ## What the gate's evidence actually was
#
# The comment above `SIZE_GATE = 2048` says "2-4KB is 0/10 and >4KB is 0/14". Re-measured over
# the whole fleet history, **only 8 above-2KB functions have ever been first-touched** -- 4
# virgin, 4 near-miss -- and none converted. The 10 and 14 were attempts, not distinct
# functions, and the sample is a quarter of what the comment implies.
#
# First-touch conversion by size band, fleet-wide:
#
#     <1KB    3400 touched   410 matched   12.1%
#     1-2KB    421 touched    14 matched    3.3%
#     2-4KB     17 touched     0 matched    0.0%
#     >4KB      18 touched     0 matched    0.0%
#
# The 2-4KB and >4KB rows are still 0%, so the gate is not contradicted. But **8 functions is
# not a basis for closing 206 functions and 674,472 B**, which is 38% of all outstanding bytes.
#
# It was also self-sealing: the gate blocks exactly the functions needed to grow the sample.
# Four virgin functions above 2 KB have ever run, ever.
#
# ## What the gate was costing, which had never been measured
#
# oc1's band is (2048, inf) -- entirely above `SIZE_GATE`:
#
#     oc1 band inventory      214 functions
#     of those servable         8
#
# **oc1 could not reach 206 virgin functions in its only band, and had 0 virgin reachable.**
# The fleet was not being protected from bad work above 2 KB; one family was idle while holding
# 674 KB of untouched work.
#
# ## Why a probe and not a change
#
# Both directions are arguable on thin data: 0/8 says the pool is worthless, 674 KB says it is
# too big to abandon. So the gate stands and `OVERSIZE_PROBE = 0.25` sends a quarter of each
# batch past it. If those convert, the gate was wrong and should move; if they do not, the probe
# is the evidence the original claim never had. Either way the question gets settled with data
# instead of inherited numbers, and reverting is one constant.
#
# Verified `OVERSIZE_PROBE = 0` reproduces the previous behaviour exactly -- zero oversize picks
# -- so the probe is additive and reversible.
#
# ## A real bug found while wiring this up
#
# The lift-above-band step deduped with `id(r) not in seen_ids`. `lifted` and `eligible` are
# built by separate comprehensions over `base`, so the same row is a different dict object in
# each and that test is **always true**: every lifted row was duplicated, and the downstream
# `inband` guard then read a list that no longer matched what had been prepended. It is the
# reason batches were stuck at 9 of 12 with 0 virgin picks while 307 qualifying virgin rows sat
# in `base`. Dedup is now by symbol.
#
# That bug predates this work -- it was introduced with the tier lift in finding 287 -- and it
# was invisible because the pick counts looked plausible. Nothing in the repo tests `choose()`.
#
# ## Batch composition now, measured
#
#     batch of 12   tier  mid  oversize  virgin  total
#     cline           3     3         3       6     12
#     oc1             3     3         3       3     10   <- band is >2KB, so less virgin fits
#     oc4             3     3         3       6     12
#     gpt             3     3         3       6     12
#     agy             3     3         3       6     12
#
# All four pools reach a session, batches fill, no symbol is selected twice across sizes 4, 6, 8,
# 12 and 16. oc1 returning 10 of 12 is correct rather than a shortfall: its band excludes the
# 1-2 KB virgin pool, so it has fewer rows to draw on.
#
# ## Still unmeasured
#
# The probe's conversion rate, and the mid band's, are both unknown until sessions land. Every
# share here is a hypothesis with a measured justification for existing, not a measured
# optimum.