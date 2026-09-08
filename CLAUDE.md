# F-Zero GX (GFZE01) matching decompilation

Matching decompilation of F-Zero GX built on decomp-toolkit / dtk-template.
Targets: `sys/main.dol` plus fifteen RELs (fourteen shipped as `files/fze.*.rel`
and `main.rel`, which `tools/prepare_orig.py` derives from `enemy_line/line__.bin`).
`ninja` must always end with `16 files OK`.

## Setup (once)

```sh
brew install ninja
# put the disc image in orig/ (RVZ/ISO), then:
uv run tools/prepare_orig.py --version GFZE01   # extracts targets, derives main.rel
python3 configure.py --version GFZE01 && ninja  # downloads dtk/objdiff/wibo/compilers on first run
uv run tools/fzgx.py sync                       # loads 7,308 functions into build/fzgx/ledger.db
uv run tools/fzgx.py restore                    # (optional) load state/ledger.json history
```

## Agent workflow

Matching is done by many cheap subagents, one function each, through the
`fzgx` CLI only (`tools/fzgx.py`): `claim` → `context` → edit `src/<unit>.c` →
`check` (≤ 8 times) → `submit` or `release`. The orchestrator picks functions
with `fzgx inventory --status unmatched --max-size N`, fans out `matcher`
subagents (`.claude/agents/matcher.md`), then runs the `librarian` once per
wave, then `triage` for functions at the attempt cap. Details: SPEC.md (local,
not committed) and `docs/CODING_RULES.md`.

Rules that hold for everyone:

- Never commit anything from `orig/` or `build/`; never commit compilers.
- Never push. Commits are local; the user pushes.
- `fzgx submit` is the only path that flips a unit to `matching`; it relinks and
  verifies every hash before committing.
- Matchers edit only their own `src/<unit>.c`. Headers, names and splits belong
  to the librarian.
- No hardcoded addresses (`fzgx lint`), no inline asm in `src/`.

## Layout

- `config/GFZE01/` — dtk config, per-module `symbols.txt`/`splits.txt`, `units.json` (generated units, read by `configure.py`).
- `src/dol/`, `src/rel/<module>/` — C units; `include/` — shared headers.
- `tools/fzgx/` — ledger, carve, oracle, context, lint; `tools/linecrypt.py`, `tools/avlz.py`, `tools/prepare_orig.py`.
- `docs/batches/` — per-milestone and per-batch reports; `state/ledger.json` — committed ledger snapshot.
- `tests/` — `uv run --group dev pytest -q` (no game data needed).
