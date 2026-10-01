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

The host assigns explicit unique symbols through orchestrate.py. Atomic SQLite claims, private source copies, and shared-unit/pending reservation exclusion prevent overlapping assignments. Models receive source, assembly and initial diff, and exactly five function-bound operations: write_unit, patch_unit, check, read_evidence, release. The host binds identity and handles submission and serialized 16-target hash verification. Accepted source is committed locally. Never push automatically.

GPT uses the constrained Codex app server. Claude uses restricted mode, no builtin tools, strict MCP and no interactive permission prompts. Cline uses its genuine SDK and existing account, five supplied tools, no model/native tools, sequential execution and high reasoning. AGY uses a session-local HOME with the existing account state and only the bound MCP server, plus a PreToolUse deny guard; personal broad project plugins/configuration are not merged. Its native tools are denied rather than prompt-trusted.

Eight checks, three stale checks, three attempts, ten-minute sessions, twenty-five-minute batch deadline and five-minute activity watchdog bound work. Per-function response-event usage guards are 256,000 input / 32,000 output tokens. Batch guards are 1,000,000 input / 80,000 output / 12 MiB logs. Counters are durably persisted; response-event guards can overshoot within a response. Missing usage after two minutes stops the batch. Failures back off independently and survive restart. Runtime adapter changes invalidate schedule context; accepted commits do not themselves justify repeated attempts. Empty eligible inventory idles without model calls.

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

## Verified rollout evidence

Initial constrained batches accepted fn_12_38340 (38c65f05), fn_12_2EE7C (7e11f357); subsequent fleet telemetry records three unique verified symbols, last e7ce845b. This is not a promised throughput rate.

The four-provider policy/adapter scratch acceptance suite passes nine checks; the updated controller suite passes ten. Python compile and Node syntax checks pass. Real Ninja, DTK report `16 files OK`, and fzgx lint reports zero findings. No mock tests are added to the repository.

Live restart shows Opus 5.5 High and GPT 6.1-Sol Medium making checked attempts; Space Bunny Alpha High emits genuine model usage through its SDK. AGY's real request is blocked by account quota and autonomously schedules retry using the returned reset duration. Do not report AGY as productive while quota-blocked. Read live status for current counts; these are not static guarantees.
