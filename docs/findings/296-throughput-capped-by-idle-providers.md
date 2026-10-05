# 296 -- throughput is capped by two idle providers, not by work selection

#
# Post-change verification, 2026-10-05 ~11:30, after the probe and pool-share changes went live
# and the daemon restarted (PID 469966, started 11:21 vs the 11:03 edit).
#
# ## The selection changes are live and behaving
#
# The new daemon is dispatching the intended pools and batches fill 12 of 12:
#
#     family   tier   mid  oversize  virgin  total
#     agy        1      0        2       5      6
#     oc1        1      0        2       2      3    <- band is >2KB, less virgin fits
#     oc4        1      0        2       5      6
#
# Oversize work is reaching sessions for the first time. Live claims after the restart:
# `fn_1_138A04` (oc4) and `fn_10_75D4` (agy), both outside the old tier-only pattern.
#
# ## But the fleet is running at 17% of its own concurrency cap
#
#     FLEET_CAP                        18 sessions
#     configured ceiling               12 (cline 6, gpt 3, agy 1, oc1 1, oc4 1; claude off)
#     actual concurrent                2-3
#     mean session                     6.2 min
#     observed throughput              32 sessions/hour
#
# 6.2-minute sessions mean each concurrent slot yields ~10 sessions/hour, so:
#
#      3 slots  ->  29 sessions/hour   (observed)
#      5 slots  ->  48
#      8 slots  ->  77
#     18 slots  -> 174
#
# **Concurrency is the only lever that matters for speed, and two providers are not
# contributing.** cline is configured for 6 and delivers 0-1; gpt is configured for 3 and is
# rate-limited. agy, oc1 and oc4 are the only families actually working.
#
# ## cline aborts ~99% of its sessions
#
#     date     aborted  completed  failed
#     10-01        260           2      17
#     10-02        528           7      37
#     10-03        412           5      76
#     10-04        214           1      56
#     10-05         19           1      12
#
# ~1,433 aborted against 16 completed over five days. The sessions are not wasted -- best
# candidates are saved and cline still produced 10 matches in 24h -- but almost all of its
# budget is spent on work that is thrown away mid-session.
#
# This is **not** the NULL-`best_percent` crash of finding 288 (fixed 08:50, and the aborts
# continue unchanged before and after). The evidence says provider-side:
#
# - No error event is logged. All tool calls report `ok:true`; the log simply stops.
# - No fixed trigger: aborts are spread across 1-22 turns, median 7 minutes, against a
#   30-minute `SESSION_TIMEOUT`.
# - It is batch-wide, not per-session: 14-16 of 16 sessions abort in the same batch.
# - One session inspected in detail (`fn_1_53E40`) ran 6 turns and 12 tools, all finishing
#   cleanly, reached 98.1% over 4 checks, then emitted `"status":"aborted"` with no error and
#   no usage.
#
# The generic `harness crash (rc=1); saved best candidate automatically` note is what made
# this invisible: it implies a harness fault, when the harness ran fine and the session was
# terminated from outside.
#
# ## Surge is firing and cannot help further
#
# `surge_clock` shows `surge: True` with `down_accum` at 44.5 hours, so the paid-provider
# fallback already engaged. But `SURGE = ('oc1','oc4')` at `parallel=1` each is the entire
# surge pool -- 2 sessions. Raising `FLEET_CAP` or waiting for the trigger changes nothing;
# the ceiling is the size of the surge group and the parallel setting inside it.
#
# ## Recommended order, by measured effect
#
# 1. **Fix or retire cline.** It is the largest configured block (6 of 12 slots) and delivers
#    the least. If the aborts are an account or endpoint problem, that is an operator action;
#    if they are the SDK wrapper, `fleet_cline.mjs` is where it shows. Until then, dropping
#    cline's parallel to 1 stops advertising capacity that does not exist.
# 2. **Raise surge parallelism.** `oc1`/`oc4` at parallel=1 each cap the fallback at 2
#    sessions while paid providers are down. They are the best converters of the four
#    opencode families (15.1% and 14.5% by the note above `SURGE`), so 2 each is 4 more
#    sessions for the same provider cost.
# 3. **Re-enable claude or accept the ceiling.** `enabled: false` is deliberate and
#    documented, but it removes a documented 14.4 matches/hour from the standing fleet. Worth
#    a cost decision, not a technical one.
# 4. **Only then tune pool shares.** `TIER_SHARE`, `MID_BAND_SHARE` and `OVERSIZE_PROBE` are
#    all still hypotheses. Doubling throughput at the wrong mix is faster at being wrong.
#
# The honest summary: the fleet is not short of work or short of good targeting. It is running
# about a fifth of its configured capacity because one provider is aborting everything and the
# fallback that should cover for it is capped at two sessions.