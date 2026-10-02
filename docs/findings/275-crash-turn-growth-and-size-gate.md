# 275 — digging into the crashes, the token growth, and the size-gated pool

Date: 2026-10-02. Follows 274 (955b14e0). Windows 00:00-08:00 / 08:00-15:35, fleet-v2 only.

## (a) Crashes are narrow, and mostly already fixed

`fleet_cline.mjs:71` aborts the agent as soon as a terminal tool call lands, so
`status: "aborted"` is the **normal success path** for a match or release, not a failure.
Only `status == "failed"` exits 1 (`fleet_cline.mjs:95`), and that set is small:

| family | failed runs | of total | rate |
|---|---:|---:|---:|
| cline | 27 | 404 | **6.7%** |
| gpt | 0 | 491 | 0% |
| oc1-4 | 0 | 404 | 0% |
| claude | 0 | 53 | 0% |

So the 127 "crashes" decompose as: 58 provider refusals (955b14e0 reclassifies 51 of
them as `rate-limited`), **27 genuine cline failures**, and the rest runs still in flight
(993 logs have no final `result` event).

The 27 real cline failures emit **no error event and no message** - `text` is empty and
the run simply stops mid-sentence. They are not turn-limit failures: median turns are 9
for `failed` versus 11 for successful `aborted` runs. They correlate with the longest runs
(usage up to 928k input). This looks like a provider-side stream drop in the
`stealth/space-bunny-alpha` SDK path, not something the harness can classify or prevent.

Damage is bounded: **43 of 127 crashed attempts kept a saved best body**, and those are
exactly the 43 that had run checks. The 84 that did no work are almost all the instant
refusals from 955b14e0. Crashes burn 8.0% of the day's input tokens (24.6M of 308.2M) -
real, but not the main story.

## (b) Token cost is quadratic in turns, and it is the reasoning trace

Per-turn **incremental** input for cline, measured from `usage-updated` events:

| turn | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| incremental input | 13.0k | 26.5k | 33.9k | 40.9k | 47.3k | 51.5k | 57.3k | 68.3k | 75.4k |

It grows ~6-8k tokens *per turn* because every turn re-sends the entire conversation, and
each turn appends the assistant's reasoning, its rewritten unit, and tool results. Median
11 turns; cumulative input 682k per attempt.

The prompt is **not** the problem - median initial prompt is 30,675 bytes (~7.5k tokens).
Nor is the tool payload: on live claimed functions the median work copy is 2,866 bytes and
a `check` response 1,924 bytes, ~1.3k tokens combined - roughly 5x smaller than the
observed per-turn growth. The remainder is the model's own `high`-effort reasoning
(median 53.5k output tokens per attempt, re-sent as input every subsequent turn).

Because only `gpt` has prompt caching (47% hit; every CLI harness 0%), this quadratic
term is billed at full rate on cline/oc. The structural fixes are, in order of value:

1. **Prefer `gpt`**, where the same history is ~10x cheaper on repeat turns. Already the
   highest-yield family too.
2. **Fewer turns per attempt** - every turn saved removes a full re-send of everything so
   far. The stale cap already bounds this (median 4.9 of 5), so this is a policy knob, not
   a bug.
3. **Cache or compact the transcript** for CLI harnesses. Nothing in the repo does this
   today.

Lowering reasoning effort would cut the growth directly, but CLAUDE.md states explicit
fleet policy: "No silent cheaper-model or lower-effort fallback." That is a policy
decision, not a tuning knob, and is left alone deliberately.

## (c) The size gate is mostly justified; the exception is real but small

Match rate by function size:

| size | total | matched | rate |
|---|---:|---:|---:|
| <=256 | 4737 | 4476 | **94.5%** |
| 257-512 | 1029 | 668 | 64.9% |
| 513-1024 | 847 | 265 | 31.3% |
| 1025-2048 | 454 | 47 | 10.4% |
| >2048 | 245 | 31 | 12.7% |

That curve is partly circular - gated functions were never attempted. The honest test is
what >1024B functions yield *when they do get attempts*, which `size_allowed` permits at
`best >= 95`:

| band | attempts today | matched | yield |
|---|---:|---:|---:|
| <=1024 | 846 | 137 | 16.2% |
| 1025-2048 | 17 | 2 | **11.8%** |
| >2048 | 6 | 0 | **0.0%** |

So the gate is **well justified above 2048B** (0% yield on 6 attempts) and **only
marginally justified in 1025-2048B**, where 11.8% is close to the 16.2% baseline. The
untapped pool is 621 functions: 407 in 1025-2048 and 214 above 2048. Of those, only 58
have `best >= 90%`, and the medians are 73.3 and 68.3 - i.e. mostly hard, not near-misses.

n=17 and n=6 are far too small to justify moving a gate. The right next step is a
controlled batch on the 1025-2048 band only, with `SIZE_GATE` raised for that band, and a
pre-registered stopping condition - the yield has to beat 16.2% on a real sample or the
gate stays. Do not widen it on this evidence.

## Summary

- Crashes are a cline-provider problem (27 runs, 6.7%), not a harness bug; work is
  preserved and they cost 8% of tokens. Already mitigated at the classification layer.
- Token cost is the re-sent reasoning trace, quadratic in turns, uncached on every CLI
  harness. Route work to `gpt`; reduce turns; cache/compact the transcript.
- The size gate holds above 2048B; 1025-2048B deserves a measured experiment, not a
  change made on 17 data points.