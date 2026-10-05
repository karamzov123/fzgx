# 291 -- items 2 and 3 of finding 289 re-checked: one was already done, one is the same bug

#
# Finding 289 recommended four items. Working them in order:
#
# **#1 (register the 16) was wrong and broke the build** -- see finding 290, reverted, hash gate
# restored to 16 files OK. **#2 (re-band) was already done.** **#3 (fn_12_23410) is a
# missing-symbol link failure, not a source problem.** #4 deferred to the fleet at the user's
# instruction.
#
# ## #2: the bands are already correct
#
# 289 said "re-measure and re-band the families against the actual virgin inventory, which starts
# at 1 KB", implying the bands still pointed at exhausted shelves. They do not. `FAMILY_BANDS`
# was corrected in an earlier session and its comment already records the exact fact 289
# rediscovered:
#
#     # The whole virgin supply sits in two bands: 330 functions at 1-2 KB and 206 above 2 KB.
#     # Nothing below 1 KB is untouched (0 virgin at <=256, 257-512 and 513-1K), so any family
#     # banded there is structurally starved and will fall through to re-deriving near-misses.
#
# Re-measured live, and the bands partition the supply exactly:
#
#     virgin <1KB     0 functions
#     virgin 1-2KB  318 functions  457,408 B   -> cline (1024,2048), agy (512,2048)
#     virgin >2KB   206 functions  674,472 B   -> oc1 (2048,inf)
#     oc4 and gpt take wide bands deliberately, to absorb whatever the narrow ones leave.
#
# No change made. Re-deriving this from the same table would have been churn, and the band
# comment is more informative than a new measurement would be.
#
# ## #3: fn_12_23410 is the same defect as #1, not a separate mystery
#
# `fzgx why-link fn_12_23410` reports the object **identical** -- `size_retail 298552 ==
# size_ours 298552`, no text diffs, no section diffs -- and `relink_rc: 1`:
#
#     While resolving relocations in 'build/GFZE01/movie_module/movie_module.plf'
#     Failed to find symbol fn_12_5674 in any module
#
# `fn_12_5674` appears in **no config file**. `grep -rl fn_12_5674 config/` returns nothing; it
# exists only in built objects and the `.plf`. And `fn_12_5674` is referenced by
# `build/GFZE01/src/rel/movie_module/fn_12_D0B8.o`, where `fn_12_D0B8` is **one of the 16
# unfinished asm units** from #1.
#
# So the chain is: an unfinished asm unit leaves a hole in movie_module, the hole is a symbol the
# REL step cannot resolve, and every consumer of movie_module -- including the 100%-matching
# `fn_12_23410` -- fails to link. **Fixing the 16 correctly should close `fn_12_23410` too**,
# which is the strongest argument for doing #1 first and then re-testing #3 rather than
# treating #3 independently.
#
# `fn_12_23410` has cost 13 attempts and 4 link-mismatches against a 100.0% object. Every one of
# those sessions re-derived a source that was already correct and then failed at the link. No
# amount of further source work on that function can help; the blocker is one missing symbol.