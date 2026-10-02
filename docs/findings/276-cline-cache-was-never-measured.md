# 276 — cline is NOT uncached: the cache-rate table in 274/275 is an instrumentation artifact

Date: 2026-10-02. Supersedes the cache claims in 274 and 275. Fleet-v2, cached input
billed at 0.1x.

## The claim being corrected

274 concluded "**only gpt gets prompt caching (47% hit); every CLI harness 0%**", and
275 repeated it, concluding gpt was "~40% cheaper per attempt" than cline and
recommending re-weighting work away from cline on token grounds.

**Both the 47% and the 0% are measurement bugs.** The real numbers are gpt **86%** and
cline **95%** — cline is the *more* cached of the two.

## Why: `cacheReadTokens` is discarded for cline only

The cline SDK does emit cache counters. `fleet_cline.mjs:83-84` drops them:

```js
if(e.type==='usage-updated'){
  usage={inputTokens:e.usage.inputTokens||0,outputTokens:e.usage.outputTokens||0};
```

The event carries `cacheReadTokens`, `cacheWriteTokens` and `totalCost`
(`@cline/core` `xf.translate`: `{type:'usage',inputTokens,outputTokens,cacheReadTokens,
cacheWriteTokens,reasoningTokenCount,totalCost}`). `fleet_provider.py:47` then reads
the same two fields. Every cline row in the ledger therefore has `cost_usd=0` and no
cache column — the harness *cannot* see a cache rate it never recorded.

`codex_server.py:406` by contrast persists `cachedInputTokens` /
`cacheWriteInputTokens` into `<symbol>.usage.json`, so gpt was the only family with
visible cache data. **The 0% was an artifact of only instrumenting one harness.**

## Recovering the discarded data

The SDK's own session store retains it: `~/.cline/data/sessions/*/*.messages.json`,
per-message `metrics.cacheReadTokens`. 829 sessions / 19,139 turns.

| family | raw input | cache read | hit rate | effective billed |
|---|---:|---:|---:|---:|
| cline | 907.3M | 857.8M | **94.6%** | 135.2M (14.9%) |
| gpt | 226k/attempt | 196k/attempt | **86.5%** | 50k/attempt (22.2%) |

cline: p10 82.0%, median 90.2%, p90 96.7%; only 3 of 829 sessions (0.4%) had no cache
read at all. Excluding my own lead session: 821 sessions, 94.6% cached — so this is fleet
behaviour, not one session.

Two corrections follow:

1. **cline's hit rate is 94.6%, not 0%.** It caches slightly *better* than gpt.
2. **gpt's hit rate is 86%, not 47%.** 274 computed `cached/(input+cached)`, but
   `inputTokens` is already cache-**inclusive** — verified directly: turn N reads exactly
   turn N-1's `inputTokens` (20615 -> 25373). Dividing a subset by input+subset halves
   the rate. Correct form is `cached/input` = 85.1% on that sample; the doc's 46.0% is
   the same arithmetic error.

Sanity check that `inputTokens` is inclusive: `totalTokens` 167745 = input 164933 +
output 2812, so cache is nested inside input, not added on top.

## Corrected efficiency (fleet-v2 only, cache-aware)

| family | attempts | raw in | effective | matches | eff tok/match | yield |
|---|---:|---:|---:|---:|---:|---:|
| claude | 116 | 18M | 5M | 36 | **146k** | 31.0% |
| gpt | 217 | 69M | 15M | 51 | **298k** | 23.5% |
| cline | 616 | 287M | 43M | 67 | **637k** | 10.9% |
| oc3 | 67 | 34M | 20M | 10 | 2.05M | 14.9% |
| agy | 52 | 52M | 31M | 17 | 1.85M | 32.7% |

Uncertainty is concentrated in assumptions, not in the measurement: oc/agy cache rates
are assumed 0.6 (no session store exposes them), claude 0.30. cline and gpt are
measured. Even if oc/agy were fully cached they stay 3-6x worse per match, so the
ranking is robust to that assumption.

## Consequences

1. **cline is still ~2.1x worse than gpt per match (637k vs 298k), but not for the
   reason given.** The cause is *turn count and yield* (10.9% vs 23.5%), not caching.
   274's mechanism — "billed full rate because only gpt caches" — is false.
2. **Re-weighting away from cline is still justified, on yield and tokens-per-match
   grounds.** The recommendation survives; the reasoning must be restated, because a
   cache fix would not recover the gap.
3. **gpt remains the best value among the always-available families** (298k/match,
   23.5% yield, zero genuine failures). claude is better still (146k/match) but quota-
   blocked until Oct 5, per 274.
4. **Fix the instrumentation before the next comparison.** `fleet_cline.mjs` must
   record `cacheReadTokens`/`cacheWriteTokens`/`totalCost`, and cline's `cost_usd` must
   stop being a structural 0. Any cross-family token claim made before that is
   unmeasured on at least one side. Expect gpt's recorded cost to rise too — it is
   currently priced from a rate table, not from provider billing.

## Method notes

- Sessions are keyed by `origin.sessionId`; the lead session is excluded so fleet data
  stands alone.
- Effective billed = `raw - 0.9*cached_read`. A provider-side cache-write surcharge
  would raise this; cline reported `cacheWriteTokens=0` throughout.
- Cache ratios come from per-message `metrics`, which the SDK writes from the raw
  provider usage object, so they are provider-reported rather than inferred.