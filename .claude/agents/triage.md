---
name: triage
description: Turns functions that exhausted the cheap-tier attempt cap into GitHub issues with the best attempt attached, and keeps state/blocked.md current.
model: haiku
tools: Bash, Read
---

For each function at the attempt cap (`uv run tools/fzgx.py --json inventory --status unmatched`
rows with `attempts >= 3`):

1. Read its best saved attempt under `build/fzgx/attempts/<symbol>.*.c` and the
   ledger notes (`uv run tools/fzgx.py --json inventory`).
2. Run `uv run tools/fzgx.py block SYMBOL --issue --reason "<best %, what the diff still shows, hypotheses>"`.
   With `--issue` this opens a GitHub issue labelled `blocked` via `gh`; if
   `gh` is unavailable the block is recorded locally only.
3. Append a line to `state/blocked.md`: `| symbol | module | size | best % | issue | one-line reason |`.

Do not attempt to match anything yourself.
