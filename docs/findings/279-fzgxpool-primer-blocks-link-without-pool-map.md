# 279 — the `.fzgxpool` primer is unreachable when there is no pool mapping

Date: 2026-10-03. Affected so far: 6 confirmed, 5 of them at 100%.

## The bottleneck

A layout primer is emitted into every saved body that declares file-scope objects of the
retail TU, in a section named `.fzgxpool`. It exists to make the **per-object oracle**
agree: MWCC emits anonymous-section objects in first-access order, so the primer pins that
order and the compiled object matches retail byte for byte.

The module **link**, however, rejects `.fzgxpool`:

    ### mwldeppc.exe Linker Error:
    #   multiply-defined: 'lbl_3_bss_A2410' in fn_3_17098.o
    #   Previously defined in auto_05_0006D8C0_bss.o

`poolfix.apply` is what drops that section (`poolfix.py:268`), and it runs only under
`res.matched_pool` (`oracle.py:223-225`, and `api.py:760` for the submit-side reconfigure).
A body whose object matches with `pool_rows: 0` — no private-literal retargeting at all —
therefore **never reaches the code that would drop its own primer**. The primer is only
stripped on the path that does not need it.

Measured on `fn_3_17098`: object `100.0%`, `matched: True`, `pool_rows: 0`,
`pool_map: {}`, and every submit rejected with `outcome='link-mismatch'`. Removing the
primer *and* the two globals it collided with keeps the object at exactly 100%:

    fn_3_17098: 100.0%  unit=  MATCH

and `ninja build/GFZE01/ok` then prints `16 files OK` with **zero differing bytes**
against `orig/GFZE01/files/fze.customize.rel`.

## Two separate traps, worth not confusing

1. **`.fzgxpool` unknown-section warnings are harmless.** The linker prints
   "Section '.fzgxpool' is unknown. Section ignored." on *every* REL link and still
   returns 0. Reading those warnings as the failure wastes a bisect.
2. **A stale hash read is not a mismatch.** `verify` relinks and immediately runs
   `shasum -c`. Under fleet load the read can land mid-write and report
   `customize.rel: FAILED / 15 files OK` while the file on disk is byte-identical to
   retail. Re-running `build/tools/dtk shasum -q -c config/GFZE01/build.sha1` by hand
   returns `16 files OK`. Compare the bytes (or re-run the checker) before believing a
   link rejection.

## The recipe

For a body that is object-perfect and link-rejected:

1. Read `.fzgx/attempts/<sym>.linkfail.*.c` and confirm `percent: 100.0`.
2. Check `pool_rows`/ `pool_map` in the check store. **Zero means the primer will never
   be dropped for you.**
3. Delete the `.fzgxpool` block, keeping the file-scope declarations it guards; add
   `types.h` and the module headers for any struct types they use.
4. Link once and delete exactly the globals the linker names as `multiply-defined`.
   Those are private copies of storage a split data object already owns.
5. Re-check. If it is still 100%, the object survived the cleanup and the recipe holds.

Worked example archived at `.fzgx/attempts/fn_3_17098.PRIMERLESS-100.c`.

## Cohort

Six functions sit in this state (>=99.5% object, link-rejected, preserved body carries a
`.fzgxpool` primer): `fn_3_17098`, `fn_1_7E8F4`, `fn_1_C6F8C`, `fn_1_FC760`, `fn_8_704`
(all 100.0%), and `colchg_selmate_disp` (99.61%). The five at 100.0% are byte-complete
reconstructions that no amount of matcher effort can advance.

## Why this is a tooling bug, not a decompilation problem

Every consumer that selects saved work — the repair corpus, the next attempt's context,
`stuck.best_bodies` — reaches bodies through `attempts.best_body_path`. `verify` now
writes that row (finding 272), so these bodies are at least *visible*. But visibility is
not enough: the corpus keeps re-offering a body whose only defect is tooling that will
not strip its own primer, and the fleet re-derives the same object from a stub each time.
The fix belongs where the primer is generated (`fixup_layout.py`) or where `poolfix` is
gated (`oracle.py`), not in a matcher prompt.

Result: unmatched functions at exactly 100.0% went 7 -> 7, with one (`fn_3_17098`)
proven closable by the above recipe and its corrected body archived.