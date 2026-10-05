# 288 -- claiming a never-attempted function crashed the claim worker

#
# Measured 2026-10-05, fleet-v2, trailing 12h: 309 attempts, 3 matches, and **179 harness
# crashes**. Yield had fallen from 5.2% (24h) to 1.0% (12h) to 0.6% (6h). This is the cause.
#
# ## The bug
#
# `build_context` formatted the ledger's `best_percent` as a float unconditionally:
#
#     parts.append(f"- attempts so far: {row['attempts']}  best: {row['best_percent']:.1f}%")
#
# `best_percent` is NULL for any function nobody has attempted. **624 unmatched functions
# have NULL**, which is every virgin target the fleet is steered toward. Claiming one raised
#
#     TypeError: unsupported format string passed to NoneType.__format__
#
# in the claim worker (`cli.py:801 worker_main` -> `api.claim` -> `context.build_context`).
# The exception fired while assembling the claim, so **the claim was never recorded** -- the
# session died before it could do any work, and the symbol stayed unclaimed.
#
# The numbers are not ambiguous:
#
#     attempts on NULL-best symbols : 220 of 309
#     crashes  on NULL-best symbols : 161        (73% crash rate)
#     crashes  on attempted symbols :  18
#
# So the fleet was losing roughly three of every four virgin sessions to a format string, on
# precisely the pool it had been re-steered toward. That is why authored code sat at 30.56%
# while the fleet reported itself healthy.
#
# ## Why it went unnoticed
#
# The crash was recorded as `harness crash (rc=1); saved best candidate automatically` --
# 175 of the 179. That message is accurate and useless: it implies a harness fault and a
# preserved candidate, when in fact the harness never ran and there was nothing to save. Only
# 59 attempts carried a real traceback, because the other 120 were summarised to the rc line.
# The traceback, not the outcome field, held the diagnosis.
#
# The bug predates the retargeting work in findings 283/287. Steering more sessions at virgin
# 1-2 KB work increased exposure to a defect that had been quietly discarding them, so the
# retargeting looked like a failure when it was partly this.
#
# ## Fix
#
# Print the truth instead of formatting a float over nothing:
#
#     best = row["best_percent"]
#     best_text = "none yet" if best is None else f"{float(best):.1f}%"
#
# Verified end-to-end through the real `api.claim` path on two NULL-best symbols
# (`fn_3_3AE8`, `fn_9_AF0`): both now claim successfully and emit `- attempts so far: 0
# best: none yet`. The pre-fix expression is confirmed to raise `TypeError` on the same row.
# No other site formats `best_percent` as a float.
#
# ## Note on the timeline
#
# This fix is in `context.py`, which the claim worker imports per invocation, so it is live in
# running sessions without a daemon restart. The fleet-multi targeting fix (finding 287) is
# different: `fleet_multi` is imported once in-process by `fleet.py daemon`, so it needs a
# restart, which drains in-flight work. Do not conflate the two when checking whether a change
# is live -- compare the daemon start time against the file mtime.