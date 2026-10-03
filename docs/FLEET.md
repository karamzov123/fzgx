# New fzgx autonomous fleet

This is `~/projects/fzgx`, not the legacy NATC/PM fleet. The enabled user service `fzgx-fleet.service` runs `tools/fleet.py daemon`, whose entrypoint delegates to `fleet_multi.py`. Do not enable the conflicting old `fzgx-autopr.service`.

## Explicit model policy

Four standing providers, independently controlled, with no silent fallback:

| Provider | Requested model/effort | Runtime identifier |
| --- | --- | --- |
| Claude | Opus 5.5 High | claude-opus-5-5, high |
| GPT | 6.1-Sol Medium | gpt-6.1-sol, medium |
| AGY | Gemini 3.8 High | gemini-3.8-flash-high, high |
| Cline | Space Bunny Alpha High | stealth/space-bunny-alpha, high |

AGY's installed authenticated model catalog recognizes that identifier. Quota failure remains quota failure, not permission to choose a different model. Its reset time is parsed for automatic retry.

## Fleet-managed surge capacity (opencode)

Four `oc1`..`oc4` families run `opencode/space-bunny-free` at variant `xhigh` on the
same contract as every other matcher: one function per session, six bound tools, no
shell, no filesystem. They are **not** standing fleet and are not operator-controlled
(`fleet.py toggle oc1` is refused with an explanation). They are enabled and retired
automatically from measured paid-provider availability, in `fleet_multi.apply_surge`:

- A paid provider is *down* when it is rate-limited, in error, or switched off by the
  operator. Idle, starting and blocked providers are not down.
- Two or more down starts a dwell timer. After **600 s cumulative** down time the four
  come up. Cumulative, not wall-clock, so a single five-minute outage buys nothing
  while a provider that keeps lapsing and re-limiting does count.
- While up, capacity is surrendered only after **1800 s** of all providers healthy, and
  re-arming then requires fresh evidence.
- The dwell lives in `runtime-v3.json`, so a service restart cannot reset it.
- `FLEET_CAP` (18) bounds total sessions across all families; if the standing fleet
  leaves no room the tile says so rather than starting a partial group silently.

The opencode transport has three constraints that are load-bearing and easy to
regress: the client runs from a directory outside `$HOME` (opencode loads `AGENTS.md`
upward, and `~/AGENTS.md` is an unrelated project's contract); `XDG_CONFIG_HOME` points
at a shared private config root that exposes only the six bound tools (writing the
config without redirecting `XDG_CONFIG_HOME` leaves the session with a full shell); and
the binary is resolved explicitly to the official client, because the free models answer
403 `FreeTierError` on any other build. A runtime guard in `fleet_provider.run` kills any
session whose first non-`fzgx_*` tool call appears.

## Execution contract

The host assigns explicit unique symbols through orchestrate.py. Atomic SQLite claims, private source copies, and shared-unit/pending reservation exclusion prevent overlapping assignments. Models receive source, assembly and initial diff, and exactly six function-bound operations: write_unit, patch_unit, check, search, read_evidence, release. The host binds identity and handles submission and serialized 16-target hash verification. Accepted source is committed locally by the existing verifier. Worker/model transports never push. A separate user-authorized deterministic publisher forwards verified local main commits to the fork; it does not integrate candidates.

GPT uses the constrained Codex app server. Claude uses restricted mode, no builtin tools, strict MCP and no interactive permission prompts. Cline uses its genuine SDK and existing account, five supplied tools, no model/native tools, sequential execution and high reasoning. AGY uses a session-local HOME with the existing account state and only the bound MCP server, plus a PreToolUse deny guard; personal broad project plugins/configuration are not merged. Its native tools are denied rather than prompt-trusted. AGY print mode soft-denies `call_mcp_tool` without a grant, so the session-local `settings.json` allows exactly `mcp(fzgx/<tool>)` for the five bound tools; without it every session ends `incomplete` with zero checks. Cline's per-response output cap is 32,768 tokens: at 8,192, high-effort reasoning truncated 32 of 209 turns before any tool call.

Sixteen checks, five stale checks (the matcher core's defaults), three attempts and forty model turns bound a session. There are no token guards (2026-10-01): cumulative input counts re-read cached context on every turn, so the former 256,000-token guard killed productive Claude sessions at seven checks, reported them as crashes, dropped the rest of the batch and triggered exponential backoff. Provider quota rejections keep their own cooldown. Time limits are hang detectors only: 30 minutes per session, 15 minutes without any log event in a batch, and a batch deadline scaled to its function count. A runaway-log guard (64 MiB per session slot) remains. Cline's SDK reports usage only when a response ends; there is no missing-usage watchdog. A batch that finishes cleanly without a match is not a failure and is followed immediately by fresh work. `FZGX_MAX_MODEL_INPUT_TOKENS`/`FZGX_MAX_MODEL_OUTPUT_TOKENS` still work when set; 0 disables them. Failed/quota backoff still survives restart. Runtime adapter changes invalidate schedule context; accepted commits do not themselves justify repeated attempts. Empty eligible inventory idles without model calls.

A real Ninja and DTK hash gate runs before work. Bootstrap health records credit zero matches. Singleton ownership, orphan recovery, graceful signal draining, mixed systemd kill mode and bounded restarts prevent duplicate hosts and preserve candidate work. Existing old branches/worktrees are preserved.

## Free-tier capacity (2026-10-03)

Cline (`stealth/space-bunny-alpha`) and the opencode surge families
(`opencode/space-bunny-free`) are free with no usage limit for a few more days. That
changes the allocation rule, and it is worth stating plainly because the obvious
efficiency ranking points the other way.

Measured over the overnight window (2026-10-02 18:00 onward), per family:

| family | attempts | matches | rate | worker-hours | matches/hour | cost |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| `gpt` | 269 | 51 | 19.0 % | 10.72 | **4.76** | quota |
| `oc2` | 31 | 5 | 16.1 % | 4.96 | 1.01 | free |
| `oc1` | 60 | 8 | 13.3 % | 11.08 | 0.72 | free |
| `oc4` | 70 | 8 | 11.4 % | 12.70 | 0.63 | free |
| `cline` | 379 | 31 | 8.2 % | **73.34** | **0.42** | free |

On efficiency alone this says cut Cline: 0.42 matches/hour against GPT's 4.76, an 11x gap,
and Cline alone consumed 62 % of all fleet hours for 30 % of the matches. **That reasoning
is wrong while Cline is free.** Efficiency per hour only decides allocation when hours are
the scarce input. Cline's hours cost nothing and are unlimited, so its low conversion rate
costs nothing either — it is spare capacity that happens to be slow. Cutting it would buy
back wall-clock time that can then be spent on... nothing, because GPT cannot absorb it:
its throughput is quota-capped, not slot-capped.

So the rule while the free tier lasts: **push work onto the free families and leave the
quota families at whatever their quota allows.** GPT stays at `parallel 2` (its cap);
Cline was raised 6 -> 8. Surge capacity is deliberately not operator-scalable — it comes
and goes with paid-provider availability — so the standing free family is the only lever.

Two things bound this, and both are real:

- **Shared build lock.** Every check serialises on it. Cline sessions are long (about 700 s
  each, against GPT's 143 s), so raising Cline's parallelism can delay a GPT submit. GPT is
  the highest-yield family, so it must not be starved. Watch GPT throughput; if it drops,
  back Cline off. `active` slots stay within `FLEET_CAP` 18.
- **This is a time box.** When the free tier ends, this rule inverts and the efficiency
  table above becomes the right basis. Re-measure then; do not leave Cline at 8 by default.

Worth noting at the time of writing: GPT was `rate-limited`, i.e. the paid providers were
down and the free families were the only capacity actually running.

The fleet stopped committing for 70 minutes: four provider batches each ended with an unlocked `snapshot` + `git add state/ledger.json`, and `snapshot` truncated the 13 MB file in place. A rewrite under another batch's `git add` kills git with SIGBUS (reproduced: 32 of 60 adds), git does not remove `.git/index.lock` on that signal, and every later `git add` exits 128, so each verifier failed, every family was held and backed off, and one pool match stayed uncommitted. Now `Ledger.snapshot` replaces the file atomically, the batch-end snapshot commit holds `submit.lock` like the verifier, and both call `oracle.clear_stale_index_lock()` first (lock older than 30 s and no git process in the repository). Symptom to recognise: all families `error` with `Hash verification failed; saving and holding producers.` while `gate.log` says `16 files OK`; read the batch's `verify.stderr.log`.

## Deterministic search in the loop (2026-10-01)

Measured on the day's sessions: about 70% of a near-miss's residual rows were register allocation, agents permuted declarations by hand at one check per guess, and the repair engine ran only at release with 2 rounds x beam 2 in 6 seconds. The same engine at 12 rounds x beam 6 compiles 6,000-14,000 variants in 12-18 seconds.

- `api._search_body` is that deep search (150 s budget, three concurrent slots). It runs at release for any body at 80% or better, before a retry's model request (`claim --repair`, bound fleet claims only), and on demand through the sixth tool `search` (three uses per attempt; an exact result is accepted, a better body replaces the work copy, otherwise nothing changes and no check is spent). A searched body gets `<body>.searched.json` (engine hash, variants); the same body is not searched again until the engine changes. When a search improves a body without closing it, the improved body is what the next attempt receives.
- Check output marks rows `L` when only a section/pool base or a displacement off it differs (`stuck.layout_rows`) and states how many rows are the matcher's, by kind. The retry context lists only those rows, says when the search has already exhausted permutations, and lists edits earlier sessions compiled without gain (`context._tried_edits`).
- A failed compile spends a check but not a stale check; fewer differing rows than the attempt has seen resets the stale counter (the score metric changes once pool rows appear).
- Functions past the three-attempt cap with a saved body at 90% or better that the current engine has not searched are eligible once more (`fleet_multi.unsearched_near_misses`); the release marker ends that. The claim cap is therefore enforced by selection, not by `--max-attempts`.
- Restored upstream history names saved bodies under `/Users/rayan/fzgx/.fzgx/attempts/` that are not on this machine: of 567 capped functions at 90%+ under 1 KiB, 464 have no local body. Those upstream attempts (DeepSeek, effort-none and Luna batches) no longer count toward this fleet's cap: selection counts only `fleet-v2-*` attempts (`fleet_multi.local_attempts`), which raised the eligible pool under 1 KiB from 434 to 1,390 functions and its 90%+ share from 15 to 579 (a 2026-10-01 snapshot; both pools shrink as the fleet matches and caps functions). Without a local body they start from the mechanical draft; the committed repair archives hold C for only a few of them.
- First result: `fn_14_82E4` (99.0% under Claude and AGY) matched after one `search` (register rows and pool primed) plus two structural edits. One of those undid an engine bug, fixed here: the one-field carrier rewrite renamed text inside string literals (`"%d"` became `"%d.value"`).

## What counts as an attempt (2026-10-02)

`local_attempts()` counts a `fleet-v2-*` attempt only if it ran a check, spent tokens,
or matched. A provider refused for quota, credits or transport still writes an *ended*
attempt row, but one with no checks and no tokens: no evidence and no work. Those rows
were consuming a slot against the three-attempt cap and retiring the function from
selection. 139 such rows existed when this was measured (58 codex, the rest
agy/claude/cline/opencode), and about 27 functions were at the cap on nothing but those
rows; fixing the predicate returned them to the pool with no data migration. Both
numbers move while the fleet runs. Upstream history (below) is still excluded for the
original reason.

Provider refusals are also now read correctly. The Codex app-server reports them as
events (`{"method":"error","params":{"error":{...}}}`) and Claude as a result row;
`provider_error_text` previously matched neither shape, so a usage limit extracted no
text, was classified a generic error, and got a 60 s backoff instead of the rate-limit
path. Reset times are now taken from the provider: `try again at Oct 2nd, 2026 1:08 AM`,
`try again at 1:08 AM` (date omitted), `resets 1am`, and `Resets in 46h40m56s` all
resolve to the stated wall clock.

Broken-batch detection reads terminal outcomes from the ledger
(`fleet_multi.batch_outcomes`). It used to read `<run dir>/results.json`, a filename
nothing has ever written — the codex transport writes `results.jsonl` and the bound
harnesses write no results file at all — so the list was always empty and a batch whose
every session crashed was treated as a clean batch that matched nothing, relaunched
three seconds later, indefinitely.

## Eww and diagnostics

The wrapper `~/.config/eww/scripts/fzgx-agent-fleet.py` execs the project venv. Icons show green recent claim activity, amber starting/idle, red stalled/error, grey disabled and an hourglass for quota cooldown. Activity is not a match claim. Tooltips include selected model, symbols, checks, batch matches, unique verified fleet total and retry time. Left toggles each family; right/up increases sessions; down decreases them; middle opens logs.

Use:

    systemctl --user status fzgx-fleet.service
    ~/.config/eww/scripts/fzgx-agent-fleet.py status
    journalctl --user -u fzgx-fleet.service -n 50 --no-pager
    eww poll fzgx-agents

Claude stream events named `rate_limit_event` are not themselves failures. `allowed` and `allowed_warning` permit requests; only `rejected` indicates an actual quota rejection. Quota classification examines structured error fields rather than assistant/tool text or event names. A captured live-log regression and allowed/rejected/error cases verify this boundary. The false Claude cooldown was cleared and a fresh Opus session performed a compiler check after restart.

State/control/history/runtime live in `~/.cache/fzgx-agents/*-v3.json`. Evidence remains under `.fzgx/runs/fleet-v2-*`; that prefix is retained for progress provenance. Gate log is `~/.cache/fzgx-agents/gate.log`. Eww reload can close the bar: inspect active-windows, reopen bar if needed, and poll before reading cached state.

## Non-competing fork integrator

`tools/fleet_integrator.py` is a deterministic publisher, not another decompilation agent. The existing `fzgx verify` watchers remain the sole owners of candidate acceptance and source commits. The publisher does not drain pending candidates, manipulate claims, stage files, commit, merge, rebase, reset, or force-push.

`fzgx-integrator.timer` runs a single oneshot approximately every two minutes. It requires local main and the exact origin fetch/push URL `https://github.com/karamzov123/fzgx.git`. It checks remote main ancestry and blocks on unknown/divergent history. It rejects obvious retail/auth artifacts in outgoing commits. This filename screen is not a general secret scanner; never commit secrets or proprietary binaries.

For a new SHA it acquires the existing submit.lock then build.lock nonblockingly. Busy locks yield immediately to workers/verifier. Pending ledger/dependency records and tracked/index changes defer publication without modifying them. Untracked personal files are preserved and never staged. A clean unchanged HEAD must pass Ninja's GFZE01/ok target, an explicit 16-target DTK hash check, and lint. Failed gates are persisted and not rebuilt for an unchanged commit. Success persists a receipt for that exact SHA; unchanged verified commits do not rebuild during network retries.

Compiler locks are released before network operations. A normal explicit-SHA fast-forward push updates only fork main, and an exact ls-remote readback is required before reporting publication. Newer local commits wait for the next tick. Network timeouts/auth failures produce status and retry on the timer; interactive login is prohibited. Divergence never triggers a merge or force push. The old fzgx-autopr.service stays disabled/conflicting.

Reproducible service templates live in tools/systemd/. Operational state and gate log live in `~/.cache/fzgx-agents/integrator-v1.json` and `integrator-v1.gate.log`.

    .venv/bin/python tools/fleet_integrator.py status
    systemctl --user status fzgx-integrator.timer
    journalctl --user -u fzgx-integrator.service -n 20 --no-pager

To stop automatic publication without stopping matching:

    systemctl --user disable --now fzgx-integrator.timer

Real local Git/bare-remote acceptance fixtures cover fast-forward/readback, unchanged gate suppression, dirty/index preservation, pending work, compiler-lock yielding, failed-gate suppression, changing HEAD, untracked preservation, remote divergence, destination/branch rejection and outgoing retail-artifact rejection. No unit tests are added to the project repository.

## Verified rollout evidence

Initial constrained batches accepted fn_12_38340 (38c65f05), fn_12_2EE7C (7e11f357); subsequent fleet telemetry records three unique verified symbols, last e7ce845b. This is not a promised throughput rate.

The four-provider policy/adapter scratch acceptance suite passes nine checks; the updated controller suite passes ten. Python compile and Node syntax checks pass. Real Ninja, DTK report `16 files OK`, and fzgx lint reports zero findings. No mock tests are added to the repository.

Live restart shows Opus 5.5 High and GPT 6.1-Sol Medium making checked attempts; Space Bunny Alpha High emits genuine model usage through its SDK. AGY's real request is blocked by account quota and autonomously schedules retry using the returned reset duration. Do not report AGY as productive while quota-blocked. Read live status for current counts; these are not static guarantees.
