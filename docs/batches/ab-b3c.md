# A/B: Haiku 4.5 (Claude Code) vs GPT-5.6 Luna (Codex) on the same 13 functions (2026-09-08)

Batch b3c: 13 untouched main.rel functions, 8–96 bytes. Haiku ran live
(matches committed); Luna ran afterwards in shadow mode on the identical set
(same oracle, same context bundle minus prior-attempt stats, nothing committed).
Budget rules identical: 8 checks, stop after 2 non-improving checks.

| | Haiku 4.5 | Luna |
|---|---|---|
| Matched | 7/13 | 9/13 |
| Checks | 33 | 25 |
| Input tokens | 1,803,384 | 1,670,373 |
| Output tokens | 139,631 | 24,112 |
| Cost (list) | $1.43 | $0.11 |
| $/match | 0.20 | 0.012 |

Luna matched every function Haiku matched plus fn_1_9A578 (Haiku 90.5%) and
fn_1_12E424 (Haiku 67%); its releases were at 84.7% and 95.4% (Haiku's at
0–90%). Neither matched fn_1_27DE4 or fn_1_4DE04.

Prices: Luna $0.20/$0.02/$0.25/$1.20 per M (input/cached/cache-write/output),
Haiku $1.00/$0.10/$1.25/$5.00 (platform.openai.com/docs/pricing, Anthropic list).
Per-call context: Codex 13–15k after disabling plugins/apps/browser/computer-use;
Claude headless 6.3k baseline but 6x the output (thinking).

Caveats: Luna ran second (nearby-matched context may include Haiku's
batch-mates); n=13.

Decision: Luna/Codex is the default cheap tier from here; Haiku stays available
via `--harness claude`.
