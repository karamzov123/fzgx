# 293 -- three pools had no route to a session, and one of them is oc1's entire band

#
# Finding 289's item #4 was "the 348 at 80-95% have no owner; let the fleet pick them up".
# Measured before changing anything: **choose() selected 0 of them, for every family.** So
# "whatever is open" was not true of that band -- nothing in the policy would ever select it.
#
# ## The mid band is a proven source, not a dead shelf
#
# Grouping every function the fleet has ever matched by its best percent *before* the fleet
# first touched it:
#
#     <80%   872        95-99%  194
#     80-95%  378        >=99%   64
#
# 378 functions have converted out of the 80-95% band against 194 from the near-miss bands the
# policy does prioritise. It is the second-largest proven source of matches and had no route.
#
# ## Two gates had to be opened, and both were invisible in the code
#
# 1. **Ordering.** Virgin-first put this band behind all 523 virgin functions, so the queue
#    never reached it. Simulating the one-word tier drained still gave 0/8, which ruled the tier
#    out and confirmed virgin-first was the cause.
# 2. **The size bands.** 126 of the band's 149 eligible functions are under 1 KB and every
#    family band starts at 1 KB or higher, so oc1's band (2048+) excluded **all** of them and
#    the others saw at most 23. Same mistake as the tier in finding 287: a band is a division of
#    labour, not a quality gate.
#
# ## Strict precedence is also wrong, and both ways were measured
#
# Ranking the mid band above virgin gave virgin **0 of 12** picks -- and virgin is the largest
# proven source at 872 conversions. Ranking virgin above the mid band is what starved it in the
# first place. An unbounded tier took 7 of every 12 slots. So the pools are bounded shares
# (`TIER_SHARE`, `MID_BAND_SHARE`) with virgin taking the remainder.
#
# Two implementation traps, both found by measuring the pick counts rather than by reading:
#
# - Slicing the pools in the list did not bind. The lockout block had already partitioned rows,
#   so tier rows sat *after* the head and a nominal cap of 4 still yielded 9 tier picks of 12.
# - The fallback must exclude rows the caps declined. Leaving them in the tail meant the caps
#   were cosmetic, and also that the fallback had nothing to offer: it contained 0 virgin rows.
#
# ## The finding that matters most: oc1 cannot do virgin work at all
#
# `SIZE_GATE = 2048` admits a function only if it is under 2 KB **or** already at >=95%. The
# comment above it is explicit that this is deliberate and evidence-based ("2-4KB is 0/10 and
# >4KB is 0/14"). But the consequence was never reconciled with the bands:
#
#     oc1's band            (2048, inf)     -- entirely above SIZE_GATE
#     virgin above 2 KB     206 functions, 674,472 B
#     oc1 band, virgin reachable    0
#
# **oc1 has 206 virgin functions in its only band and can reach none of them.** That is not a
# starved pool, it is an idle family: every oc1 session so far has been re-deriving near-misses
# or mid-band work because that is all `size_allowed` lets through.
#
# The gate's own evidence is old (2-4KB 0/10, >4KB 0/14 -- a handful of attempts) and the
# fleet has run thousands of sessions since. Re-measuring 2-4 KB and >4 KB conversion now, and
# either raising the gate for virgin work or narrowing oc1's band, is the change to make. It is
# a policy decision about where to spend the fleet, so it is left for the operator rather than
# changed on the strength of one measurement.
#
# ## Not yet verified
#
# The mid band now dispatches, but its **conversion rate is unmeasured**. 378 historical
# conversions say the pool is not dead; they do not say a session spent on it today converts at
# that rate against virgin work at its current rate. Re-run the yield comparison in both pools
# once a few hundred attempts have landed before treating `MID_BAND_SHARE` as correct.