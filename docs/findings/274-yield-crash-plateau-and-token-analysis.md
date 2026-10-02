# 274 — yield analysis: crash causes, plateau exits, and where the tokens go

Date: 2026-10-02. Tooling fix: 955b14e0. Measured over the two windows 00:00-08:00
and 08:00-15:35, fleet-v2 batches only.

## Window comparison

| | 00:00-08:00 | 08:00-15:35 |
|---|---:|---:|
| attempts | 361 | 508 |
| matched | 66 | 68 |
| yield | 18.3% | 13.4% |
| tokens_in / attempt | 281,889 | 406,456 (+44%) |
| plateau releases | 121 (33%) | 220 (43%) |

Throughput is flat (~9 matches/h) while yield fell. The second window spent 40% more
attempts for the same output.

## A measurement error worth recording

The first reading of this was "69% of attempts go to the 85-99.5% band and return almost
nothing", which would have pointed the whole effort at near-miss triage. **That was
wrong.** It banded attempts by each function's *current* `best_percent`, and a function
that eventually matches is trivially near 100 now. Reconstructing the score *before* each
attempt (running max of `best_in_attempt` in id order) inverts it:

| prior score | attempts | matched | yield |
|---|---:|---:|---:|
| <85 | 679 | 124 | **18.3%** |
| 85-95 | 17 | 1 | 5.9% |
| 95-98 | 39 | 3 | 7.7% |
| 98-99.5 | 90 | 4 | 4.4% |
| 99.5-100 | 44 | 7 | 15.9% |

The fleet already puts 78% of its attempts where the yield is. **Selection is not the
problem.** Any tuning here must reconstruct prior state, never read current state.

## Crash causes (fix 955b14e0)

127 attempts were recorded as `crash`. By duration: 58 under 15s, 19 at 15-60s, 16 at
1-5m, 34 over 5m. The 58 instant ones are provider refusals, not harness failures — a
quota/rate refusal exits `rc=1`, and the outcome was `rc == -9 ? timeout : rc == 0 ?
incomplete : crash`. `claude-opus-5-5` crashed 15 of 16 times this morning for exactly
this reason: **its weekly limit is exhausted** (`"You've hit your weekly limit · resets
Oct 5, 4pm"`, `rateLimitType: seven_day`, `utilization: 1`, `org_level_disabled`). It is
not broken and will not return before Oct 5 — re-enabling it cannot raise today's yield.

Refusals are now classified `rate-limited` and, like crash/timeout, stop queued launches
so the supervisor backs off. Verified: the claude limit log is caught, a normal aborted
cline run and an empty output are not, and 0 of 1172 cline/oc logs are flagged.

The remaining ~69 crashes are genuine harness failures, concentrated in cline (56).

## Plateau exits: do NOT raise MAX_STALE

Plateau is the largest terminal outcome (43% of attempts, mean `stale_checks` 4.9 against
a cap of 5). The case for raising the cap looks strong at first - overnight, 65% of
plateau exits scored >=98. But exits almost never convert afterwards:

| exit score | n | matched since | rate |
|---|---:|---:|---:|
| <90 | 10 | 1 | 10.0% |
| 90-95 | 5 | 0 | 0.0% |
| 95-98 | 28 | 1 | 3.6% |
| 98-99.5 | 48 | 3 | 6.2% |
| 99.5-100 | 32 | 6 | 18.8% |

Even a 99.5-100% plateau exit matched only 18.8% of the time *given unlimited further
attempts*. These functions are structurally stuck, not under-checked, so more checks buy
tokens, not matches. The stale cap is doing its job. This also shifted by window: exits
were 65% >=98% overnight and only 27% >=98% this morning, so the same knob would look
correct in one window and harmful in the next - a reason to leave it alone rather than
tune it to the latest snapshot.

The right owner for this population is the deterministic engine, per CLAUDE.md:
"Plateaus are data, not agent work."

## Where the tokens go

| family | model | yield (win1) | yield (win2) | crash | cache hit |
|---|---|---:|---:|---:|---:|
| claude | opus-5-5/high | **44.4%** | 0% (quota) | 15/16 | 0% |
| gpt | gpt-6.1-sol/med | 10.0% | **20.6%** | 0 | **47%** |
| agy | gemini-3.8-flash/high | 34.2% | off | 11/38 | 0% |
| cline | space-bunny/high | 11.3% | 12.7% | 41, 15 | 0% |
| oc1-4 | space-bunny/xhigh | 0-40% (n<=5) | 3.6-16.1% | 20/133 | 0% |

**Only `gpt` gets prompt caching (47% hit rate).** Every CLI harness re-sends the full
context at full price. Effective billed input per attempt, counting cached tokens at
~0.1x:

- cline: 385.9k
- gpt: 232k

so `gpt` is ~40% cheaper per attempt *and* yields 20.6% vs 12.7%. It dominates cline on
both axes.

The initial context is not the problem: median prompt is 30,675 bytes (~7.5k tokens).
The cost is cumulative re-read - each turn re-sends the whole conversation, so spend
scales with turns x growing history. `cline` went 287k -> 476k per attempt (+66%) with no
change to its context builder, which points at conversation growth (evidence reads,
diff payloads) rather than the prompt.

## Consequences

1. **Scale `gpt`.** `parallel: 1`, 20.6% yield, zero crashes, ~40% cheaper per attempt.
   8 of `FLEET_CAP=18` slots are in use; `claude`/`agy` are disabled and `gpt` is
   currently rate-limited.
2. **Re-weight from `cline`**, which takes 45% of all attempts at 12.7% yield and the
   highest token cost per attempt.
3. **Do not raise `MAX_STALE`** on this evidence.
4. `claude` is unavailable until Oct 5; do not spend effort on it today.
5. Untapped: 599 unmatched functions (>1024B, <95%) are unschedulable under
   `SIZE_GATE=1024` - 33% of remaining work, completely unmeasured.