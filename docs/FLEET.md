# New fzgx autonomous fleet

This is `~/projects/fzgx`, not the legacy NATC/PM fleet. The enabled user service `fzgx-fleet.service` runs `tools/fleet.py daemon`, whose entrypoint delegates to `fleet_multi.py`. Do not enable the conflicting old `fzgx-autopr.service`.

## Explicit model policy

All four providers are enabled, independently controlled, with no silent fallback:

| Provider | Requested model/effort | Runtime identifier |
| --- | --- | --- |
| Claude | Opus 5.5 High | claude-opus-5-5, high |
| GPT | 6.1-Sol Medium | gpt-6.1-sol, medium |
| AGY | Gemini 3.8 High | gemini-3.8-flash-high, high |
| Cline | Space Bunny Alpha High | stealth/space-bunny-alpha, high |

AGY's installed authenticated model catalog recognizes that identifier. Quota failure remains quota failure, not permission to choose a different model. Its reset time is parsed for automatic retry.

## Execution contract

The host assigns explicit unique symbols through orchestrate.py. Atomic SQLite claims, private source copies, and shared-unit/pending reservation exclusion prevent overlapping assignments. Models receive source, assembly and initial diff, and exactly five function-bound operations: write_unit, patch_unit, check, read_evidence, release. The host binds identity and handles submission and serialized 16-target hash verification. Accepted source is committed locally by the existing verifier. Worker/model transports never push. A separate user-authorized deterministic publisher forwards verified local main commits to the fork; it does not integrate candidates.

GPT uses the constrained Codex app server. Claude uses restricted mode, no builtin tools, strict MCP and no interactive permission prompts. Cline uses its genuine SDK and existing account, five supplied tools, no model/native tools, sequential execution and high reasoning. AGY uses a session-local HOME with the existing account state and only the bound MCP server, plus a PreToolUse deny guard; personal broad project plugins/configuration are not merged. Its native tools are denied rather than prompt-trusted.

Eight checks, three stale checks, three attempts, ten-minute sessions, twenty-five-minute batch deadline and five-minute activity watchdog bound work. Per-function response-event usage guards are 256,000 input / 32,000 output tokens. Batch guards are 1,000,000 input / 80,000 output / 12 MiB logs. Counters are durably persisted; response-event guards can overshoot within a response. Other providers retain the two-minute missing-usage guard. Cline supports eight parallel sessions (fleet control ceiling twelve); its per-function emergency limits are 1,000,000 input / 128,000 output tokens with five minutes for initial usage telemetry. Cline aggregate token allowances scale with assigned function count, and logs with session count; a full eight-session, sixteen-function batch allows 16,000,000 input / 2,048,000 output tokens and 96 MiB logs. Only the SDK owning an over-budget function aborts/releases that candidate; its siblings are not stopped by supervisor per-function accounting. Aggregate safety, check/stale/time limits remain. Failed/quota backoff still survives restart. Runtime adapter changes invalidate schedule context; accepted commits do not themselves justify repeated attempts. Empty eligible inventory idles without model calls.

A real Ninja and DTK hash gate runs before work. Bootstrap health records credit zero matches. Singleton ownership, orphan recovery, graceful signal draining, mixed systemd kill mode and bounded restarts prevent duplicate hosts and preserve candidate work. Existing old branches/worktrees are preserved.

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
