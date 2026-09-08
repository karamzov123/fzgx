# GitHub Actions and decomp.dev

The public repository never contains game files, so CI builds inside a **private**
container image that does. This follows dtk-template's pattern exactly.

## 1. The private build repository (`rayanht/fzgx-build`)

1. Create a **private** repository from the
   [dtk-template-build](https://github.com/new?template_name=dtk-template-build&template_owner=encounter)
   template, named `fzgx-build`.
2. Add the game files under `orig/GFZE01/`, **only what the build needs**: run
   `uv run tools/prepare_orig.py --version GFZE01` in this repository first, then copy
   `orig/GFZE01/sys/main.dol`, `orig/GFZE01/files/fze.*.rel` and
   `orig/GFZE01/files/enemy_line/main.rel` (the derived, decrypted REL; `line__.bin`
   itself is not needed by the build).
3. Let its action build the image, then in the package settings under
   "Manage Actions access" grant this repository (`rayanht/fzgx`) the **Read** role.

The workflow in `.github/workflows/build.yml` pulls `ghcr.io/rayanht/fzgx-build:main`,
copies `/orig` into the workspace, configures with `--compilers /compilers --binutils /binutils`,
builds `all_source progress build/GFZE01/report.json`, runs the tests, and uploads the
report as the `GFZE01_report` artifact.

## 2. decomp.dev

Once the workflow is green on `main`, register the project at
<https://decomp.dev/manage/new> with this GitHub repository. decomp.dev reads the
`GFZE01_report` artifact of every run on the default branch and renders:

- shield badges: `https://decomp.dev/rayanht/fzgx.svg?mode=shield&measure=code&label=Code&category=all`
  (`category` is `all`, `dol`, `modules`, or one of the extra categories in
  `configure.py`: `game`, `sdk`; `measure` is `code` or `data`);
- the live treemap: `https://decomp.dev/rayanht/fzgx.svg?w=1024&h=320`;
- per-version badges: `https://decomp.dev/rayanht/fzgx/GFZE01.svg?mode=shield`.

The README already embeds these URLs; they render once the first report is ingested.

## 3. What the report counts

`report.json` is objdiff's whole-project measurement: matched code bytes and functions
per unit, split into the categories above. Units are the per-function and per-TU C
files in `config/GFZE01/units.json`; unmatched code is linked from decomp-toolkit's
split objects and counts as unmatched. There is no hand-written assembly anywhere, so
"matched" means matched in C.
