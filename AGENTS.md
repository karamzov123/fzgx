# Agent contract (Codex and any other harness)

This repository is driven by a small CLI so that any agent harness can do the
same work. Read `CLAUDE.md` for the project overview; this file is the
contract a subagent must follow. The Claude Code versions of these roles live
in `.claude/agents/*.md` and say the same things.

## Matcher (one function per session; model: GPT 5.6 Luna or equivalent cheap tier)

Given `SYMBOL` and `AGENT_ID`:

```sh
uv run tools/fzgx.py claim SYMBOL --agent AGENT_ID        # carves src/<unit>.c
uv run tools/fzgx.py context SYMBOL                        # retail asm, symbols, neighbours, idioms, rules
# edit src/<unit>.c  (only this file; no hardcoded addresses; no inline asm)
uv run tools/fzgx.py check SYMBOL                          # ≤ 8 times; prints % and a target|ours diff
uv run tools/fzgx.py submit SYMBOL --agent AGENT_ID --harness codex --model gpt-5.6-luna --message "<one line>" [--names names.json]
# or
uv run tools/fzgx.py release SYMBOL --reason "<what is left>"
```

End your output with `RESULT: matched|released SYMBOL <percent>%`.

Never run ninja, dtk, objdiff or git directly. Never edit `include/`,
`config/`, or another unit. Names you infer go in the `--names` sidecar:
`[{"kind":"function|object|struct|field","target":"SYMBOL","name":"...","rationale":"..."}]`.

## Librarian (serial; stronger model)

Applies name proposals (`fzgx names`), promotes shared structs into
`include/`, merges fully matched ranges into translation units, adjusts data
ownership in `splits.txt`, appends to `docs/MWCC_IDIOMS.md`. Always ends with
`python3 configure.py && ninja` printing `16 files OK`, `fzgx lint`,
`fzgx snapshot`, and a commit prefixed `librarian:`.

## Triage

For functions with `attempts >= 3`: `fzgx block SYMBOL --issue --reason "..."`
and a row in `state/blocked.md`.

## Orchestrator

Picks work with `fzgx --json inventory --status unmatched --max-size N`
(smallest first), fans out matchers in parallel (start at 8), then librarian,
then triage; writes `docs/batches/<date>.md` from `fzgx report` and commits
`state/ledger.json` via `fzgx snapshot`. Budget per batch is enforced by the
orchestrator from the `--cost-usd` values matchers report.
