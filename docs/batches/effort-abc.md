# Effort and model A/B/C on the same 80 functions (2026-09-08)

Shadow trials (nothing committed) on 80 unmatched main.rel functions of 512–1024 B
that Luna had failed on before: arm A Luna at default reasoning effort, arm B Luna
at high effort, arm C Gemini 3.8 Flash (medium) through the Antigravity CLI `agy`.
Same instructions, same tools, same 8-check budget.

| arm | n | matched | ≥90% | mean best % | median best % | $ total | $/fn | mean s |
|---|---|---|---|---|---|---|---|---|
| A Luna default | 79 | 2 | 10 | 69.1 | 70.8 | 1.93 | 0.024 | 224 |
| B Luna high | 79 | 2 | 11 | 63.4 | 71.0 | 2.86 | 0.036 | 353 |
| C Flash 3.8 medium | 24 | 4 | 22 | 95.8 | 98.6 | 21.56 | 0.898 | 289 |

Arm C stopped at 24 of 80: the Antigravity individual subscription quota ran out
("Individual quota reached ... Resets in 4h30m"), 56 agents died without a result
(attempts not charged). On the 24 functions all three arms completed:

| arm | matched | ≥90% | mean best % | median best % | $ |
|---|---|---|---|---|---|
| A | 1 | 6 | 80.2 | 84.5 | 0.55 |
| B | 1 | 5 | 63.5 | 73.1 | 0.65 |
| C | 4 | 22 | 95.8 | 98.6 | 21.56 |

Flash beat Luna's best on 23 of the 24 (the exception is the one all three matched).
High effort on Luna bought nothing: same matches, lower mean, 1.5× slower, 1.5× dearer.

Cost is the API-equivalent at the introductory Gemini 3.8 Flash rate ($0.75 fresh,
$0.075 cached, $3.75 output incl. thinking, per 1M tokens; `agy` reports cached tokens
separately from input). What the subscription actually charges is the plan, up to its
quota. The spend is output-bound: a typical agent burns ~150K output tokens of which
half is thinking, against ~150K fresh and ~650K cached input.

Harness notes (`--harness agy`): `agy --print --output-format json`; the fzgx MCP server
is registered at user scope in `~/.gemini` with absolute paths; agents run from an empty
directory (`.fzgx/agy-cwd`) because agy has no flag that disables its shell (`--mode plan`
and `--sandbox` both still ran commands). Antigravity allows 16 in-flight requests per
account: 40 parallel crashed every agent with "exhausted your capacity"; the runner
relaunches a capacity-killed agent after a pause, and 12 parallel ran clean until the quota.

Next: finish the 56 after the quota resets; try `gemini-3.8-flash-low` to cut the
thinking share; `patch_unit` to cut the rewrite share.
