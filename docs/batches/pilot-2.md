# Pilot wave 2 — 15 no-shell matchers in parallel, Haiku 4.5 (2026-09-08)

| Function | Size | Result | Checks | Notes |
|---|---|---|---|---|
| fn_1_101448 | 4 | matched | 1 | empty |
| fn_1_FE5E0 | 4 | matched | 1 | empty |
| fn_1_FE780 | 4 | matched | 1 | empty |
| fn_1_3EF8C | 28 | matched | 1 | conditional store |
| fn_1_86194 | 32 | matched | 1 | wrapper |
| fn_1_D35B0 | 36 | matched | 1 | two calls |
| fn_1_411A4 | 48 | matched | 1 | retry with the argument-position idiom |
| fn_1_137548 | 60 | matched | 1 | prepend-arg wrapper |
| fn_1_149BF4 | 56 | matched | 1 | four BSS loads |
| fn_1_150574 | 64 | matched | 1 | struct field load |
| fn_1_AB5CC | 80 | matched | 2 | byte load, call, store |
| fn_1_8CA20 | 20 | matched (orchestrator) | 1 | agent hit 100% twice; link dropped the unreferenced unit (`-strip_partial`); fixed by force_active in carve |
| fn_1_12F128 | 40 | released 76.5% | 8 | register allocation (r4 for a global) |
| fn_1_9818C | 56 | released 68.5% | 8 | "consistent across versions" |
| fn_1_1024C4 | 52 | released 92.4% | 8 | float args f1/f2/f3 order |

12/15 matched; 15 agents ran concurrently without incident under the single
build lock. Cumulative after both waves: 23 matched, 3 released (all first
attempts, 2 retries left each), 0 blocked.

Rates so far on ≤ 96-byte main.rel functions with Haiku 4.5: 18/23 = 78% per
first attempt; 1.3 checks per match; ~1 min wall clock per agent.

Tooling changes from this wave: `carve` force-activates every carved symbol
(regression test); idiom note about `-strip_partial`.
