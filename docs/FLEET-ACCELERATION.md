# Fleet acceleration evidence and operating policy

This is a measured trial, not a claim that a model or larger fleet is superior.

## Effort and transport

`orchestrate.py --effort` exports `FZGX_EFFORT`. The restricted Claude launcher uses that value; OpenCode passes it as per-session `--variant`, retaining `--pure`, the matcher agent, and all six-tool restrictions. Codex keeps its app-server reasoning setting. Direct Claude matcher runs must use `FZGX_BOUND_TRANSPORT=1`; the unrestricted legacy command is not an acceptable benchmark.

The current trial pins are `gpt-6-luna` high and `claude-sonnet-5-5` high, one session each. Luna was resolved using live Codex `model/list`; Sonnet's alias was confirmed by provider model usage. Three existing Space Bunny xhigh lanes remain enabled. AGY, Cline and OpenCode #4 retain operator-disabled state. No CPU or memory limits were raised.

## Recorded trials

All replay trials used already-matched functions, three checks, one model/tool worker, and shadow mode. They do not contribute new closures:

| Model/effort | Target | Result | Session seconds |
| --- | --- | --- | --- |
| Luna medium | fn_1_668 | 67%, released | 11.0 |
| Luna high | fn_1_668 | exact | 20.1 |
| Sonnet medium, bound | fn_1_798 | exact | 35.2 |
| Sonnet high, bound | fn_1_798 | exact | 18.3 |
| Space Bunny medium | fn_1_668 | no stdout/tool receipt; deadline | 204.5 |

The earlier Sonnet medium non-bound invocation was a setup failure (tools unavailable), not a model-quality result. Different replay targets and single samples prohibit cross-model rankings.

Real bounded trials:

- Luna high: fn_1_6E320, six checks, eight tool calls, 135.6 seconds; best increased from 86.605125 to 91.15385%. No exact closure.
- Sonnet high: fn_1_487EC, four checks, 137.9 seconds; attempt reached 48.4%, old 86.48529% frontier preserved. No exact closure.
- Attempt-capped targets fn_1_4068C and fn_1_1384C rejected standalone assignment before inference. No cap was raised for those tests. Fleet selector eligibility and standalone claim eligibility are separate contracts.

Do not use harness-reported $0 as evidence of free inference; price/usage support varies by provider.

## Session health telemetry

Bound CLI runs write a sibling `<symbol>.phase.json`: spawn, stdout/JSON/model event, actual bound-tool receipt timestamps, tool starts/completions, exit code/time, guard reason, and explicit provider errors/status. Receipts, not model prose, establish tool activity. Writes are atomic and failure-safe. A step-start event is activity evidence, not identity verification. A deadline with no stdout does not establish whether the provider or client was responsible. Terminal code 143 without a guard reason likewise does not prove a provider failure.

Historical read-only audit found 109 OpenCode attempts in its 24-hour window, including 53 normal incomplete sessions and 24 rc=-15 interruptions. Separate these outcomes; do not attribute all interruptions to providers. New phase telemetry enables useful first-tool comparisons without retries, header spoofing, or enlarged deadlines.

## Routing and specialist evidence

Fresh saved-body compiler checks (current project defaults plus source pragmas) reproduced:

- fn_1_12DAEC: 99.66307% raw, 94.34% adjusted, 21 register-allocation residuals. Treat as allocation analysis, not a one-word closure.
- fn_1_2D038: 99.53651% raw, 99.68% adjusted, one immediate and one register residual. Its completed zero-yield spelling search is not new work.
- fn_1_135D7C: 99.54724% raw, 99.61% adjusted, one mr/extsb instruction-form residual. Use signedness/instruction-form evidence, not a whole-function rewrite.

The existing selector already reserves measured residual-tier work, widens for saved near-misses, uses mid-band work and a bounded oversize probe, and excludes link-rejected functions from object-only matcher work. Keep these controls rather than replacing them with an unmeasured score formula. Small-module leads require fresh reference/claim eligibility checks; small size is not proof of easy closure.

The proven 0x440 row layout and failed three-function typed-view cohort remain useful shared context, but delivered zero closures. Do not repeat that substitution without changed evidence. No deterministic SDK-import candidate was established by this audit.

## Follow-up: recurrence diagnostics and search evidence

The current build graph needed legitimate recovery work. A later dry run specifically found `build/tools/dtk` newer than `build/GFZE01/config.json`, forcing split/reconfiguration. No incorrect dependency edge was proven. `run_gate()` now captures fail-safe before/after `gate-inputs-*.json` and `gate-explain-*.log` in the fleet cache. Real Ninja and the independent sixteen-target hash check remain mandatory. The final post-gate dry run reported no work to do.

Three Space Bunny sessions each reached the 600-second tool-idle guard without stdout or a tool receipt. Their successor sessions also remained silent at inspection. OpenCode #1/#2/#3 were disabled through normal controls; all their claims drained. Luna/Sonnet remain enabled at one session each.

Additional high-effort shadow replays:

- Haiku `claude-haiku-5-5`, fn_1_668: exact, two checks, 92.7 seconds.
- Opus `claude-opus-5-5`, fn_1_798: exact, two checks, 15.7 seconds.
- Sol `gpt-6.1-sol`, fn_1_668: exact, three checks, 31.3 seconds.
- Haiku real fn_3_12B14: six checks, 283.6-second session, saved at 88.34952%, no closure.

These single samples support tool admission only, not a general ranking. Shared deterministic repair contributes to replay outcomes.

The deterministic repair path now stores zero-improvement, nonmatching search evidence by symbol, source contents, conservative header inventory, compiler binaries/options, reference/config/tooling identity and the exact engine hypothesis. Identical source saved under another filename shares evidence; changed inputs expire it. Missing/custom unresolved inputs fail open. Improvements, errors and zero-variant searches do not create quarantine. Budget sufficiency is required. Existing sidecars gain context keys; legacy sidecars without compilation identity are not used to suppress potentially changed-context work. This may require one bounded contextual refresh, not a blanket function ban.

A real one-second compiler probe produced one variant and no improvement; the repeat reused its evidence with zero variants. Its one-second record cannot suppress a production 150-second search. Nine external gate/helper/integration tests passed. All sixteen hashes passed after integration.

The maintenance drain uncovered an unresolved teardown issue: `stop_runner()` timed out at 120 seconds, the supervisor failed closed, and systemd removed surviving descendants. The exact remaining Codex claim was recovered via normal `release --save-only` after checking no live owners remained. No claims were cleared directly. This recovery is not a teardown fix; do not silently enlarge cleanup timeouts or restart over live owners.

Two read-only investigation agents and one helper implementation agent assisted this follow-up, all at depth 1.

After restoration, the fleet total reached 405. The authoritative ledger attributes the new verified 2,240-byte `fn_1_138144` closure to `claude-sonnet-5-5`, accepted at commit `3e74a6bc`. It came from the preceding Sonnet batch; it is not evidence that the new search cache caused the closure.

## Acceptance and next measurements

Track verified bytes and unique accepted functions per wall-clock hour, tool-reaching fraction, first-tool latency, and terminal reasons. Compare equivalent target cohorts before permanent effort/model promotion. Do not increase concurrency or CPU allocation until phase telemetry and cgroup samples show local resource pressure rather than inference waits. Preserve existing source edits and all reference/hash gates.

Raw trial logs remain under `.fzgx/runs/accel-*`; external regression tests and diagnostic reports are under `/home/armandofm/.hermes/cache/scratch/fzgx-acceleration/` (scratch is temporary). Three depth-1 agents assisted: two read-only audits and one telemetry implementation. Parent review found and fixed actual OpenCode error/event shape handling; four telemetry and two effort tests passed.
