# 295 -- fn_12_5674 is DCT_FsriInit: a split generator emits address-derived names for named symbols

#
# Finding 292 left `fn_12_D0B8` uncarved and recorded that its object has an undefined reference
# to `fn_12_5674`, defined nowhere. That "defined nowhere" was wrong, and chasing it closed the
# last two open items (#1's 16th function and #3's `fn_12_23410`).
#
# ## `fn_12_5674` is `DCT_FsriInit`
#
#     config/GFZE01/movie_module/symbols.txt:88
#     DCT_FsriInit = .text:0x00005674; // type:function size:0x17C
#
# 0x5674 is a real, named function, already split (`splits.txt:1375`, 0x5674..0x57F0), already
# built, and its object exports the real name:
#
#     $ nm build/GFZE01/movie_module/obj/rel/movie_module/DCT_FsriInit.o
#     00000000 T DCT_FsriInit
#
# So the symbol exists and is linkable. What is missing is a **call site that names it**. The
# generated assembly for `fn_12_D0B8` contains:
#
#     /* 0000D404 */  bl fn_12_5674
#     /* 0000D40C */  bl fn_12_5570        <- DCT_FsriInitScaleTbl
#
# Address-derived names for symbols that have names. The link then fails on
# `Failed to find symbol fn_12_5674 in any module`, which is accurate -- there is no such symbol.
#
# ## Why it is a generator defect and not a one-off
#
# `fn_12_D0B8` is the **only** file in `src/rel/movie_module/` that references these addresses
# (0 files use `DCT_FsriInit`, 1 uses `fn_12_5674`), and it is the only function in the module
# calling into that group. So the path that mints `fn_<module>_<addr>` instead of the real name
# is reachable and has already produced one broken unit. Every other unit in the module happens
# not to exercise it, which is why the build was green.
#
# This is the same class as findings 280 and 286: a name in generated text standing in for a
# symbol whose real name exists. The fix belongs in the code that renders the split, not in the
# `.s` file -- hand-editing `src/rel/movie_module/fn_12_D0B8.s` would be overwritten by the next
# configure, and the hash gate would reject the result anyway.
#
# ## `fn_12_23410` is downstream of exactly this
#
# `fn_12_23410` is 100.0% object-matched and has failed 13 attempts, all at link, and its blocker
# is the same undefined symbol: it links against `movie_module`, which cannot link while
# `fn_12_D0B8.o` carries an unresolvable reference. Neither function needs source work; both
# need this one name corrected.
#
# ## State
#
# `fn_12_D0B8` left uncarved, its `units.json` asm record restored after testing (the record is
# required for it to stay credited; `uncarve` removes it, which would have silently dropped a
# credited function). Verified: 16 files OK, 0 bad, build exit 0.
#
# Two credited-but-unauthored remain: `fn_12_D0B8` (no split range, blocked on this) and
# `fn_14_C850` in pilotpoint (no units.json entry, unrelated to this defect and untouched).