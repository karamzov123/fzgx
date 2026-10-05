# 290 -- the 16 credited-but-unauthored functions are unfinished asm units, not a registration gap

#
# Finding 289 recommended registering split ranges for the 16 credited-but-unauthored functions
# and called it the cheapest win on the board. **That was wrong, and acting on it broke the
# build.** Reverted; this records what actually happened.
#
# ## What they are
#
# All 16 carry `"asm": true` in units.json. They are **hand-written assembly units**, created by
# `fzgx asm-unit` for functions whose bodies cannot be expressed in C, and they are already
# registered. `progress.uncredited` reports `no split range` for all 16, which reads like a
# registration gap; it is not.
#
#     asm units in units.json          159
#     of those, with a built object    143
#     of those, without                 16   <-- exactly the uncredited set
#
# The mechanism: `configure.py` only builds asm under `--non-matching`
# (`config.asm_dir = None` otherwise), and an asm unit's `.s` is derived from its **split
# range**. With no split range there is no `.s`, so nothing is compiled, no object exists, and
# `gen_built` reports False -- while objdiff still credits the function because the **auto
# object** supplies the bytes. Verified: `target_object_for('fn_1_15578')` is
# `auto_00_00014F18_text.o`, whereas the working `fn_12_23410` targets its own `fn_12_23410.o`.
#
# So the 16 are credited *despite* having no compiled unit, not because of one.
#
# ## Why carving them breaks the link
#
# `fzgx carve` is the wrong tool. It adds the split range **and** writes a C stub, which
# `asmunit.py` then explicitly deletes (`if c_src.exists(): c_src.unlink()` -- "the carve's
# stub: the unit is assembly"). Calling `carve` alone skips that deletion, so an **empty C
# function body** survives alongside the unit. The empty body has no relocation for
# `fn_12_5674`, and the link dies:
#
#     While resolving relocations in 'build/GFZE01/movie_module/movie_module.plf'
#     Failed to find symbol fn_12_5674 in any module
#     ninja: build stopped: cannot make progress due to previous errors.
#     FAILED: all 15 .rel targets
#
# This is a trap worth naming: the single-function rehearsal passed. `fn_12_33664` carved,
# configured, built, and reported **16 files OK**, and `progress` moved uncredited 16 -> 15.
# The failure only appeared when the remaining 15 were carved in a loop. One green rehearsal is
# not evidence for a batch.
#
# ## Correct fix, not attempted
#
# Add the split range **without** a C stub, or use `asm-unit resume`. `resume()` is the
# intended recovery but is currently unsafe: it selects on `functions.status == 'asm'`, and
# **no function in the ledger has that status** (0 of 159 asm units match), so it would sweep
# all 159 -- including SDK symbols like `GXLoadNrmMtxImm` and `DecrementerExceptionHandler`.
# `resume()` needs its predicate fixed before it is safe to run.
#
# Verified reverted state: `configure.py` exit 0, `ninja` EXIT=0, **16 files OK, 0 mismatch,
# 0 missing**, tree clean.
#
# ## fn_12_23410 (#3) is the same class of problem, not a link-stage mystery
#
# `fzgx why-link fn_12_23410` reports sizes identical (298552 == 298552), zero text diffs, and
# `relink_rc: 1` with the same `Failed to find symbol fn_12_5674 in any module`. The object
# matches at 100% and the **link** fails on a symbol nothing in the config defines:
# `fn_12_5674` appears in no `config/` file, only in built objects and the `.plf`.
#
# So #3 is not a per-function source problem. It is a missing-symbol problem in the REL link,
# and it is very likely the same defect as the 16 -- `fn_12_D0B8.o` referenced `fn_12_5674`,
# and `fn_12_D0B8` is one of the 16 unfinished asm units. **Fixing the 16 correctly may fix
# `fn_12_23410` as a side effect**, which is a strong reason to do #1 first and re-test #3.
#
# ## Process notes
#
# - `why-link` runs a full relink; it exceeds a 30s command window and must be backgrounded.
# - Concurrent `carve` calls corrupt units.json mid-write (observed: `JSONDecodeError` at
#   line 131160). Carves must be serialised, not looped in parallel.
# - The hash gate is `config/GFZE01/build.sha1`, whose paths are **repo-relative**
#   (`build/GFZE01/main.dol`), not relative to the version directory.