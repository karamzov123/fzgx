# 292 -- closing the credited/authored gap, and the one function that resists

#
# Follows findings 290/291. The correct fix for the credited-but-unauthored functions is to
# carve the split range **without** a C stub, which required two code fixes first.
#
# ## Fix 1: carve must not write a C stub for an assembly unit
#
# `asmunit.make` deletes the stub carve writes (`if c_src.exists(): c_src.unlink()` -- "the
# carve's stub: the unit is assembly"). `carve` called directly skipped that, which is what
# broke the link in finding 290. `carve` now skips stub creation when units.json already
# registers the symbol with `"asm": true`, via a new `_registered_asm` predicate. The range is
# still added, which is the whole point; only the empty body is suppressed.
#
# ## Fix 2: `asmunit.resume` selected on a status nobody writes
#
# It chose units whose ledger row was not `status='asm'`. **No ledger row has that status** --
# 0 of 159 -- so it would have swept every registered asm unit including SDK symbols
# (`GXLoadNrmMtxImm`, `DecrementerExceptionHandler`) that have been linked correctly for months.
# It now selects on observable state: a split range claims the unit's `.text` and the current
# build does not already produce it.
#
# Two things had to be got right for that, both found by measuring rather than reasoning:
#
# - **Object-file existence is the wrong test.** `fn_12_33664` still had both `src/*.o` and
#   `obj/*.o` from a reverted run, so an existence check called it finished while no build rule
#   mentioned it. The test is now whether the symbol appears in the current `build.ninja`.
# - **`build.ninja` lives at the repository root**, not under `build/<version>/`, which is where
#   `p.build_dir` points. Reading the version directory finds nothing, so every asm unit looks
#   unfinished and resume selects all 159. That is how the SDK symbols got in.
#
# ## Result: 15 of 16 closed, hash gate green throughout
#
#     authored fns   3748 / 3748  (100.00% of credited)
#     authored code  739,856 B    == credited code
#     HASH GATE      16 OK, 0 bad
#
# The authored/credited gap went from 17,576 B to zero and stayed zero at every intermediate
# build. Each carve was verified for a leaked stub before the next, since that was the exact
# failure mode of finding 290.
#
# ## fn_12_D0B8 is a genuine dead end, and it is what blocks #3
#
# It carves cleanly (`--dry-run` reports `.text 0xD0B8..0xD9FC align 4`, no notes) and builds,
# but linking then fails:
#
#     While resolving relocations in 'build/GFZE01/movie_module/movie_module.plf'
#     Failed to find symbol fn_12_5674 in any module
#
# This time **there is no C stub anywhere near it** -- confirmed no stub in `src/`, and the
# failure reproduces from the assembly alone. `nm -u fn_12_D0B8.o` shows one undefined
# reference, to `fn_12_5674`, and that symbol is defined **nowhere**: not in any `config/` file,
# not in `config/GFZE01/movie_module/symbols.txt` (0 hits, and no `fn_12_56xx` symbol exists at
# all), not in any built object, not in the retail data. Only `movie_module.plf`/`preplf` name
# it.
#
# So `fn_12_D0B8` references a symbol the retail module never defines. Carving it converts a
# function objdiff credits from the auto object into one that cannot link, which is strictly
# worse than leaving it alone. It was uncarved and the other 15 kept.
#
# This also explains #3 precisely. `fn_12_23410` is 100.0% object-matched and has failed 13
# attempts at link; the blocker is the single undefined reference in `fn_12_D0B8.o`, and
# `fn_12_23410` stays unmatched until `fn_12_5674` is accounted for. **No source work on
# `fn_12_23410` can succeed**, and neither can any on `fn_12_D0B8` -- the obstacle is a symbol
# that does not exist, which is a data/symbols.txt question, not a decompilation question.
#
# ## Operational notes
#
# - Each carve runs a full configure+build (~50s for movie_module, longer for main_rel). Carving
#   16 serially is ~15 minutes; they must not be run concurrently (`JSONDecodeError` writing
#   units.json, observed).
# - Launch long carve loops with `setsid ... & disown`. Under a 30s command window the timeout
#   wrapper appears to restart, which looks like a hang but is the harness re-launching.
# - Verify the hash gate by hashing `config/GFZE01/build.sha1` directly; the ninja log does not
#   always print "16 files OK" when the build is incremental.