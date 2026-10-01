# New fzgx autonomous fleet

This is the `~/projects/fzgx` fleet, not the legacy NATC/PM fleet in
`fzero-gx-decomp`. Do not operate the old supervisor or repair its profiles when
working on these icons.

## Execution contract

`tools/fleet.py daemon` schedules explicit unique function symbols through the
existing `tools/orchestrate.py --harness codex` multiplexed app-server transport.
The model sees only five function-bound matcher tools: `write_unit`, `patch_unit`,
`check`, `read_evidence`, and `release`. The host assigns/claims the function and
automatically submits exact matches. Workers cannot run shell, edit repository
configuration, claim other symbols, or run git. Atomic SQLite claims and private
work copies prevent overlapping ownership; a serialized verifier relinks,
checks all 16 retail targets, and commits accepted source locally.

Default concurrency is four sessions, with two tool subprocess slots. A batch
contains at most twice the concurrency in distinct symbols. Candidate selection
uses live inventory, favors saved near-matches and small functions, excludes
claimed/matched/blocked entries, and never raises the three-attempt cap. The
first rollout restricts targets to 1 KiB or smaller. Context fingerprints and a
persistent schedule prevent unchanged repeated sessions; an empty queue idles
without model calls. No automatic remote push, force push, worktree rebase, or
PR merge is performed by this supervisor. Existing worker branches remain
preserved for separately verified recovery.

## Bounded work and failure recovery

- Eight checks and three stale checks per attempt; ten-minute session timeout.
- Twenty-five-minute batch deadline and five-minute no-activity watchdog.
- Per-function cumulative token guards: 128,000 input / 16,000 output.
- Batch watchdog: 1,000,000 input / 80,000 output tokens / 12 MiB log output.
- Usage counters are atomically persisted by `codex_server.py` on provider usage
  events. Reaching a per-function guard stops the batch and drains tools. These
  are response-event guards, not a guarantee that a provider cannot overshoot
  inside its current response. Missing usage telemetry for an assigned claimed
  function after two minutes also stops the batch.
- Rate/quota errors wait at least 30 minutes; overload waits five minutes; other
  failures exponentially back off. Backoff survives service restarts.
- Build/hash gate runs before each batch. An unchanged failed HEAD/context is
  held without repeated Ninja builds, including across supervisor restarts.
- A singleton lock prevents two supervisors. Startup waits for any prior bound
  host, then saves/releases abandoned claims owned by this new fleet only.
- SIGTERM waits for matcher cleanup. The service uses `KillMode=mixed` and a
  three-minute stop window, so systemd does not interrupt every tool mutation
  before the host can drain it. Service crashes have a restart-rate limit.

## Eww integration

The installed `~/.config/eww/scripts/fzgx-agent-fleet.py` is a small compatibility
wrapper that execs the project `.venv/bin/python tools/fleet.py`. The existing
bar polling and click handlers continue to use it.

GPT is the supported constrained worker transport. Claude/Cline/AGY icons stay
off and explain why; their broad-shell CLI launch modes must not be reenabled
merely by prompt instructions. Supporting another provider requires an enforced
function-bound transport and a live acceptance test, not just an MCP config.

- Green `●` plus a number: current function claims and recent matcher activity.
  Activity is not a claim that matches have landed.
- Amber `◌`: starting or awaiting verified health; amber dot: idle/backoff.
- Red `!`: stalled, blocked, error, or dead/stale supervisor.
- Grey: disabled/off. Quota cooldown has a separate hourglass indicator.

The tooltip shows exact symbols/modules, checks, completed attempts, current
batch link-verification, unique link-verified progress since this fleet started,
and last accepted commit. Cached global verifier/backlog records do not count
as this batch's work. A missing verifier record is unknown, never successful. Because the watcher is
silent when no match is pending, each batch records its actual pre-batch
16-target hash-gate success as initial health (`bootstrap_gate`), with no
credited matches. Eww's initial value also never claims that Cline is running.
A PID alone never produces the working state.

GPT left-click toggles work; right-click/wheel-up increases concurrency (max 8);
wheel-down reduces it (min 1). Changes gracefully drain a current batch before
relaunching. Middle-click opens logs. Unsupported family controls explain the
disabled state without invoking their model CLI.

## Installed service and diagnostics

`fzgx-fleet.service` is enabled for the user session and runs the project venv.
It conflicts with `fzgx-autopr.service`. The old integration watcher is disabled:
it repeatedly rebased/built conflicted branches and force-pushed, and is not
part of the bound transport's commit path.

```sh
systemctl --user status fzgx-fleet.service
journalctl --user -u fzgx-fleet.service -n 50 --no-pager
~/.config/eww/scripts/fzgx-agent-fleet.py status
~/.config/eww/scripts/fzgx-agent-fleet.py toggle gpt
systemctl --user stop fzgx-fleet.service
systemctl --user start fzgx-fleet.service
```

State/control/history/runtime are in `~/.cache/fzgx-agents/*-v2.json`; gate log
is `~/.cache/fzgx-agents/gate.log`. Bound assignments, usage, tool events,
terminal results and verification records are in `.fzgx/runs/fleet-v2-*`.
The pre-repair controller, service, style and state were backed up under
`~/.cache/fzgx-agents/repair-backup-20261001-111930/`. Worktrees and old logs
were preserved. Abandoned original worker claims were released through the
project API, preserving available saved candidates.

## Rollout evidence

The first constrained live batch accepted `fn_12_38340` (commit `38c65f05`) and
`fn_12_2EE7C` (commit `7e11f357`). Fresh full builds and explicit DTK hash checks
reported `16 files OK`. Graceful service shutdown left zero claims. Eww polling
was read back via `eww get fzgx-agents`, the bar stayed open, and its rendering
was captured with a bar-only Wayland screenshot. This demonstrates real local
progress and cleanup, not a promised overnight throughput rate.

A scratch acceptance harness checks stale-PID status, candidate exclusion,
launch flags, cooldown, batch-only accounting, durable token guards, missing
verifier health, and restart-safe unique verified totals. Per repository policy,
no mock/unit-test files are added to this source tree. Test script for this
rollout: `/home/armandofm/.hermes/cache/scratch/fzgx-fleet-acceptance.py`.
