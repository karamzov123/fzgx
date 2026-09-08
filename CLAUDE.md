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
uv run tools/fzgx.py sync                       # loads 7,308 functions into .fzgx/ledger.db
uv run tools/fzgx.py restore                    # (optional) load state/ledger.json history
```

## Agent workflow

Matching is done by many cheap subagents, one function each, through the
`fzgx` MCP server (`tools/fzgx_mcp.py`, registered in `.mcp.json`; approve it
when Claude Code asks at startup). Matchers have **no shell**: their tools are
`claim` → `context` → `write_unit` → `check` (≤ 8) → `submit` or `release`,
plus read-only `Read`. The same operations exist as `uv run tools/fzgx.py ...`
for humans, the orchestrator and the librarian. The orchestrator picks
functions with `fzgx inventory --status unmatched --max-size N`, fans out
`matcher` subagents (`.claude/agents/matcher.md`), then runs the `librarian`
once per wave, then `triage` for functions at the attempt cap. Details:
SPEC.md (local, not committed) and `docs/CODING_RULES.md`.

Rules that hold for everyone:

- Never commit anything from `orig/` or `build/`; never commit compilers.
- Never push. Commits are local; the user pushes.
- Never delete `build/` or `.fzgx/`. `.claude/settings.json` denies `rm -r`,
  `git clean/checkout/restore/reset/stash/rebase/push` and `ninja -t clean`
  for every agent in this repo. If the build looks broken, run
  `python3 configure.py && ninja build/GFZE01/ok` and report; do not "clean".
- `fzgx submit` is the only path that flips a unit to `matching`; it relinks and
  verifies every hash before committing.
- Matchers edit only their own unit, and only through `write_unit`. Headers,
  names and splits belong to the librarian.
- No hardcoded addresses (`fzgx lint`), no inline asm in `src/`.
- Symbols are addressed by name; the REL entry points `_prolog`/`_epilog`
  exist in every module and must be written `module:name` (e.g. `title:_epilog`).
- `fzgx trivial` matches single-`blr` and `li r3,N; blr` functions mechanically
  (419 landed on 2026-09-08); run it before spending agents on tiny functions.
- Readability tooling: `fzgx tu-organize` (TU directories from `tus.json`),
  `fzgx structs`/`headers` (layouts from disassembly → `include/rel/<module>/globals.h`,
  offset self-checked under MWCC), `fzgx rename`, `fzgx naming-bundle`/`naming-apply`,
  `fzgx oversize` (dtk under-sized symbols). Agent ids prefixed `revise-` rewrite a
  matched unit and keep it only if still 100%; `shadow-` run A/B trials without committing.
- Naming: `fzgx naming-bundle <tu.c>` (sources, header, retail strings, assert hints) →
  a cheap model answers JSON → `fzgx naming-apply --file p.json`, which renames symbols,
  records typedef names in `config/GFZE01/<module>/typedefs.json`, re-splits, regenerates
  every header of the module (offset self-check) and relinks. Headers are generated:
  never hand-edit `include/rel/<module>/*.h`; change the analyzer or the name map.
- Header changes can alter a matched unit's code. `ninja build/GFZE01/ok` must pass after
  regenerating; if a REL fails, byte-diff `build/GFZE01/<m>/<m>.rel` against `orig/` and
  map the first divergence to a function.
- `tools/seeds/asserts.py` → `state/seeds/asserts_<module>.json` (file:line + message per
  assert call); `tools/seeds/debug_strings.py --write` refreshes `tus.json` from `__FILE__`
  anchors (then `fzgx tu-organize` and regenerate headers).
- Batches: `uv run tools/orchestrate.py --harness codex|claude ...` (headless, one report
  per batch, `fzgx verify` relinks once at the end). Never use in-process subagents.

## Layout

- `config/GFZE01/` — dtk config, per-module `symbols.txt`/`splits.txt`, `units.json` (generated units, read by `configure.py`).
- `src/dol/`, `src/rel/<module>/` — C units; `include/` — shared headers.
- `tools/fzgx/` — ledger, carve, oracle, context, lint; `tools/linecrypt.py`, `tools/avlz.py`, `tools/prepare_orig.py`.
- `docs/batches/` — per-milestone and per-batch reports; `state/ledger.json` — committed ledger snapshot.
- `tests/` — `uv run --group dev pytest -q` (no game data needed).
