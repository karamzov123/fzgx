# Agent contract (Codex and any other harness)

This repository is driven by a small CLI so that any agent harness can do the
same work. Read `CLAUDE.md` for the project overview; this file is the
contract a subagent must follow. The Claude Code versions of these roles live
in `.claude/agents/*.md` and say the same things.

## Matcher (one function per session; cheap tier: GPT 5.6 Luna, Haiku 4.5)

Matchers get **no shell**. Their only tools are the `fzgx` MCP server
(`tools/fzgx_mcp.py`, registered in `.mcp.json` for Claude Code; for Codex add
to `~/.codex/config.toml`:

```toml
[mcp_servers.fzgx]
command = "uv"
args = ["run", "tools/fzgx_mcp.py"]
cwd = "/path/to/fzero_gx"
```

) plus a read-only file tool. Tools: `claim`, `context`, `read_unit`,
`write_unit`, `check`, `submit`, `release`. `write_unit` only accepts the unit
the caller has claimed; `submit` relinks all 16 targets and verifies every
hash before committing. The same operations exist as CLI subcommands
(`uv run tools/fzgx.py ...`) for humans and the orchestrator.

Loop: `claim` → `context` → `write_unit` (complete file: `#include "types.h"`,
externs, minimal structs, the function) → `check` (≤ 8; `versions="all"` to
probe compiler versions) → `submit(..., names=[...])` or `release(reason)`.
End the transcript with `RESULT: matched|released SYMBOL <percent>% checks=<n>`.

Rules enforced by the tools, not by prose: no hardcoded addresses (lint A1/A2),
no inline asm, no edits outside the claimed unit, no build or git access.

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
