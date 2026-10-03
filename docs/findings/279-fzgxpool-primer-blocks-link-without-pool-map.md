# 279 — the `.fzgxpool` primer is unreachable when there is no pool mapping

Date: 2026-10-03. Tool: `tools/fzgx/primerless.py`. Fixed so far: 2 matched
(`fn_1_7E8F4`, `fn_1_C6F8C`), 4 proven object-perfect.

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

**A function with no split range cannot be linked, and the per-object oracle cannot see
that.** Carving a unit for an uncovered function adds a *second* copy of it to the module —
the module grows and the hash stops matching — or the REL step fails to resolve a symbol it
expected. `oracle.check` still reports 100%, because it diffs one object against the retail
object and never builds the module. This is why these bodies look object-perfect and
link-rejected with no visible cause.

`splits.txt` is ours: `dol split --no-update` never rewrites it, so a missing range is a
real gap and is safe to add. Range = the symbol's own `start`/`size` from `symbols.txt`.

`tools/fzgx/splitgaps.py` reports them:

    python3 tools/fzgx/splitgaps.py [--module NAME] [--only-unmatched] [--json]

Note what the count is *not*: 1,422 unmatched functions have no split, which is simply the
normal state (a split appears when a function links), not a backlog of defects. The tool's
value is prospective — **check for a gap before submitting any body**, or the match will be
built, accepted at 100%, and then rejected for a reason the check never showed.

Verified on two cases:

| Symbol | Split added | Symptom before | After |
| --- | --- | --- | --- |
| `fn_8_704` (title) | `0x704..0x754` | `Failed to find symbol fn_1_14F118 in any module` | **matched**, committed `1476a20e` |
| `fn_12_23410` (movie_module) | `0x23410..0x23700` | module grew **+280 bytes** | size delta **0**; object 97.2 % |

`fn_8_704`'s `fn_1_14F118` error was a red herring: nothing in that body references it. The
REL step simply cannot resolve symbols when the module it is resolving has a range that no
split produces.

`fn_12_23410` is no longer an ownership problem, but it is also no longer a 100 % body: once
it compiled in its real split context it scored **97.2 %** (ins 4, op 2, regalloc 1).
`static inline` on its `find_entry` helper changed nothing, so the helper was already being
inlined. The earlier 100 % was measured against the auto object, not the split's retail
object — worth remembering before trusting a candidate score for a function that has no
split yet.

## The recipe for the primer itself

`tools/fzgx/primerless.py` implements this; it reproduces the hand-repaired
`fn_3_17098` body byte-for-byte.

    python3 tools/fzgx/primerless.py <symbol> \
        --header types.h --header rel/<module>/globals.h \
        [--drop NAME ...] [--extern [TYPE:]NAME ...]

1. Read `.fzgx/attempts/<sym>.linkfail.*.c` and confirm `percent: 100.0`. The tool
   picks the newest body that **still has** a primer — once a primerless body is saved,
   later saves inherit it, so "newest" is the wrong pick.
2. Check `pool_rows` / `pool_map`. **Zero means the primer will never be dropped for you.**
3. Delete the `.fzgxpool` block but **keep** the file-scope declarations it guards; add
   `types.h` plus the module headers the compiler asks for by name. Do not guess them:
   the body's struct types live in the module's per-unit header, not `<module>.h`.
4. Link once and act on exactly the globals the linker names `multiply-defined`:
   - not referenced by the body -> `--drop` (delete the definition)
   - still referenced by the body -> `--extern` (declare it, do not define it).
     The type is load-bearing: `extern s32` for a `u8` global turns a `stb` into a `stw`
     and costs the match, so pass `u8:NAME` or let the tool recover it from the
     definition before it is dropped.
5. Re-check. If it is still 100%, the object survived the cleanup.

`--drop` and `--extern` are not interchangeable. `fn_8_704` needed `--extern u8
lbl_8_bss_0` because the body *writes* that global; deleting it instead gives
`undefined identifier`, and `extern s32` gives 97.0% instead of 100.0%.

## Three traps, worth not confusing

1. **`.fzgxpool` unknown-section warnings are harmless.** The linker prints
   "Section '.fzgxpool' is unknown. Section ignored." on *every* REL link and still
   returns 0. Reading those warnings as the failure wastes a bisect.
2. **A stale hash read is not a mismatch.** `verify` relinks and immediately runs
   `shasum -c`. Under fleet load the read can land mid-write and report
   `customize.rel: FAILED / 15 files OK` while the file on disk is byte-identical to
   retail. Confirm by comparing bytes against `orig/GFZE01/files/<retail file>`
   (`main_rel` is `files/enemy_line/main.rel`), or re-run
   `build/tools/dtk shasum -q -c config/GFZE01/build.sha1`. Every "failure" in this
   cohort that was checked by hand turned out to be this.
3. **Submit one candidate at a time.** Two individually-valid units submitted together
   reported a `main_rel.rel` hash mismatch that neither produced alone. Both pass
   `16 files OK` in isolation and byte-for-byte against retail.

## Results

| Symbol | Before | After |
| --- | --- | --- |
| `fn_1_7E8F4` | 100.0%, link-mismatch | **matched** (pool) |
| `fn_1_C6F8C` | 100.0%, link-mismatch | **matched** (pool) |
| `fn_1_FC760` | 100.0%, link-mismatch | object 100%, module byte-identical; re-submit sequentially |
| `fn_3_17098` | 100.0%, link-mismatch | object 100%, module byte-identical; re-submit sequentially |
| `fn_8_704` | 100.0%, link-mismatch | object 100%; blocked by a **separate** pre-existing split gap |

`fn_8_704` is a different failure: with its primer removed the link stops complaining
about the primer and instead reports

    Failed to find symbol fn_1_14F118 in any module

`fn_1_14F118` is matched and defined in `src/rel/main_rel/sel_static_disp.c`, but the
REL resolve cannot see it. That is a split/ownership problem independent of the primer,
and the baseline build without the unit is `16 files OK`, so it is not pre-existing
 breakage. It needs `why-link`/split ownership work, not this recipe.

## Why this is a tooling bug, not a decompilation problem

Every consumer that selects saved work — the repair corpus, the next attempt's context,
`stuck.best_bodies` — reaches bodies through `attempts.best_body_path`. `verify` now
writes that row (finding 272), so these bodies are at least *visible*. But visibility is
not enough: the corpus keeps re-offering a body whose only defect is tooling that will
not strip its own primer, and the fleet re-derives the same object from a stub each time.
The durable fix belongs where the primer is generated (`fixup_layout.py:601`) or where
`poolfix` is gated (`oracle.py:223`), not in a matcher prompt.