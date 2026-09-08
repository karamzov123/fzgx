# The literal pool, and what per-function units cannot match (2026-09-08)

## Finding

MWCC pools every literal a translation unit needs (float constants written in
the code, and its own int-to-double conversion constant 2^52) once per TU in
`.rodata`. Retail functions reference the shared copy. A per-function unit
compiles its own private copy (`@N`), so its relocation never matches. In
main.rel, 527 functions reference a pooled constant shared with a neighbour;
515 of them (488 KB, a third of the module's unmatched bytes) were unmatched,
172 of those (200 KB) through the conversion constant, which C cannot name.

Two shapes, measured on the 106 releases of batch m7 whose reason cited the
pool:

| shape | count | fix |
|---|---|---|
| A. same instruction, relocation to `lbl_*_rodata_*` vs to `@N` with equal bytes | 3 (mixed with other diffs) | object patch after compiling |
| B. retail addresses the whole pool through one base register plus offsets; ours needs an extra `lis` | 34 | whole-TU compile only |
| cites constants but the remaining diff is elsewhere | 70 | ordinary matching |

## What landed

- **Context** names pooled constants with their values (`extern const f64
  lbl_1_rodata_170; // = 4503599627370496.0`); agents reference explicit
  literals by symbol.
- **Shape A is closed.** `check` classifies relocation-only pool rows, marks
  them `p` in the diff, reports the score without them, and on a full pool
  match runs `tools/fzgx/poolfix.py`: the private `@N` symbol becomes an
  undefined global named after the retail constant and the emptied private
  `.rodata` loses its ALLOC flag (an empty allocatable section still made the
  linker align by 8, four bytes that failed the hash). The unit records the
  mapping in units.json and `configure.py` compiles it through `mwcc_pool`
  (derived from the generated `mwcc` rule, so CI's compiler paths hold).
  `fn_1_4D14` links from C this way, 16 files OK.
- **Shape B needs the TU as one unit.** `fzgx tu-trial <tu>` compiles the TU
  file as a single object and scores every function against its retail
  object. objput.c (4 blocks): 4 of 4 at 100% inside the whole-TU object.
  alloc.c and camera.c do not compile whole yet: block-private declarations
  written before the headers existed collide (`extern u32 x[2]` vs the
  header's `u32 x`; a private `GameCameraEntry` copy). `fzgx tu-tidy` removes
  the duplicates the headers cover when the block still matches; the rest
  is the revise pass's job, then `fzgx tu-hoist`.
- `fzgx sweep` re-checks saved attempts: 69 released attempts matched under
  the current oracle and headers and were submitted and link-verified.
- decomp-permuter is wired (`fzgx permute`) with a powerpc-eabi objdump built
  under build/tools; it refuses relocation-only diffs it cannot see. The two
  plateaus tried were not permutation problems.

## Incident

A `fzgx verify` bisected 243 pending units while the baseline itself did not
link (the four alignment bytes above), rejected 69 of them and removed their
blocks. They were restored from the saved bodies, the whole set re-verified in
one 15-second relink, and verify now relinks a baseline with every pending
unit held back before it bisects. Cause of the broken baseline: I ran configure
and ninja by hand while a batch was running; two concurrent splits kill each
other (exit 137). Rule recorded in CLAUDE.md.

## Update, later the same day

objput.c is collapsed: after `tu-include`, `tu-tidy` and `tu-hoist-decls` its
five blocks compile as one unit at 5/5 and the unit links byte-exact in place
of the five per-function ranges (`fzgx tu-collapse`, hash-verified, revert on
failure). alloc.c compiles whole with 19/20 blocks identical; its pool function
needs a retail object of the whole TU to be scored. Typing strings as `char`
module-wide broke 29 matched blocks that had declared them `u8`, so it is a
per-TU opt-in now, and `tu-reflag` re-decides every block's include set after
a header change, down to a self-contained block carrying the declarations it
matched under, copied from the committed headers.

## Next

- Regenerate headers (small globals wider than their field are arrays now),
  tidy every TU, revise the blocks that still collide, `tu-hoist`, then
  `tu-trial` alloc.c: 18 of 26 matched, 8 left including a shape-B function.
  That TU is the testbed for accepting shape B at TU level.
- Accept shape B per function only when the whole-TU compile confirms it;
  collapse a confirmed TU to one split range.
