# 289 -- the authored/credited distinction and where the remaining work actually is

#
# Measured 2026-10-05, ledger 5709 matched / 1594 unmatched / 4 claimed / 1 blocked.
#
# ## Authored vs credited, from `progress.measure`
#
# `authored` is strictly stronger than `credited`, and the difference is the whole point of the
# metric. From `tools/fzgx/progress.py`:
#
#     credited = status[symbol] == "matched"
#     authored  = credited and units.json has an entry and gen/ was built for it
#
# - **credited** means objdiff accepts the function's bytes. Retail shipped an auto object, a
#   self-contained translation unit, or a matched pool entry -- something the linker can consume
#   without us having written anything.
# - **authored** means a C body exists, is registered in `units.json`, and `gen/` built it, so
#   the bytes come from source we control.
#
# Authored is preferred because only it is real decompilation. Credited code can vanish: change
# the translation unit and the auto object no longer exists, and there is no source to rebuild
# from. The gap is currently 17,576 B -- 739,856 credited vs 722,744 authored.
#
# ## The 16 credited-but-unauthored functions are all "no split range"
#
# `fzgx honest --uncredited` reports 16, and every one is `no split range`: objdiff credits them
# (retail auto objects), but they sit outside any split range in `units.json`, so there is no
# place to put a body. 8 are main_rel, 8 movie_module; largest is `fn_1_15578` at 3652 B.
# This is a **registration gap, not a reverse-engineering gap** -- the bytes are understood, the
# project just has nowhere to put them. Cheapest available win on the board.
#
# ## The dominant fact: the small-function shelf is exhausted
#
# Unmatched work split by size, and how many have never been attempted:
#
#     size        unmatched   virgin   already attempted
#     <512 B          464        0                464
#     512B-1K         520        0                520
#     1K-2K           397      325                 72
#     2K-4K           174      171                  3
#     >4K              39       35                  4
#
# **Every unmatched function under 1 KB has been attempted at least once. There is no virgin
# work below 1 KB at all.** Meanwhile matches have come overwhelmingly from that same shelf:
# 5,749 of the 5,709 matched functions are under 512 B, and 6,749 are under 1 KB. The easy
# wins were taken; what remains under 1 KB is what resisted repeated attempts.
#
# This has a direct consequence for fleet bands. `cline` is banded to 1-2 KB and `agy` to
# 512-2048, but there are **zero virgin functions at 512-1024**, so the bottom half of both
# bands is an already-worked shelf. cline/agy are being pointed at a range whose only content is
# capped-and-failed work, which `ATTEMPT_CAP` then excludes -- the bands are not wrong, but the
# virgin pool they were sized for no longer exists below 1 KB.
#
# ## Where the remaining bytes are
#
#     module          known  matched  matched_B     todo_B
#     main_rel         3230     2419     430992    1008284
#     main             2202     1942     384772     179512
#     customize         278      168      34928     147580
#     sel               223      124      36252     121864
#     movie_module      791      648     154216     102788
#
# main_rel is 1.0 MB of the 1.78 MB unmatched -- 57% of all outstanding bytes in one object,
# and the only module still open for milestone (b). Its todo is spread across 443 sub-1KB,
# 246 at 1-2 KB, 100 at 2-4 KB and 22 above 4 KB, so it is not one big blocker, it is 811
# functions.
#
# ## The 984 sub-1KB unmatched are not one population
#
#     80-95%      348   genuinely unfinished
#     95-99%      282   close
#     <80%        130   cold / wrong approach
#     >=99%       121   near misses, and 121 of these are the tier
#     best = 0    103   never successfully checked
#
# 348 functions stuck at 80-95% are the largest coherent group anywhere on the board. They are
# not near misses, so the one-word approach does not apply, and they are too small to be
# interesting to a model session on size alone. Nothing currently targets "wrong shape, right
# size" as a category.
#
# `fn_12_23410` is the one function at 100.00% and still unmatched: 4 link-mismatch attempts,
# 9 released. Object bytes are correct and the link fails, which is a different problem from
# every other function here. It is the only such case (141 symbols have ever hit
# link-mismatch; 140 are since resolved).
#
# ## Recommended order
#
# 1. Register split ranges for the 16 credited-but-unauthored functions. Known bytes, no
#    reverse engineering, closes the authored/credited gap outright.
#2. Re-measure and re-band the families against the *actual* virgin inventory, which starts at
#    1 KB. The current bands assume small-function headroom that is gone.
# 3. Investigate `fn_12_23410`'s link failure -- a single function, a distinct failure mode.
# 4. Treat the 348 at 80-95% as a category. It is the biggest single group and has no owner.