# 13: configure.py registration flips compiler flags — two build pipelines

Date: 2026-08-22. Discovered integrating sa-gxinit (GXMisc) + sa-mtx2 (GXInit)
waves into main. Cost ~4 build cycles.

## The two pipelines

1. **Registered** (`Object(Matching, "...")` in configure.py) → ninja mwcc
   rule → `build/GFZE01/src/**/*.o`. cflags include `-str reuse -multibyte
   -DBUILD_VERSION=0 -DVERSION_GFZE01 -DNDEBUG=1` and LACK `-common off
   -use_lmw_stmw on`.
2. **dtk auto** (src path present under a splits unit but NOT registered;
   entry auto-written to config.json) → `dtk rel make` → `build/GFZE01/obj/
   **/*.o`. Effective flags match tools/unit_check.py FLAGS
   (`-str reuse,pool,readonly -common off -use_lmw_stmw on ...`).

## Consequence

Same .c, same GC/1.2.5n, DIFFERENT codegen: GXMisc.c compiled under
pipeline 1 has GXAbortFrame at .text offset 1220; under pipeline 2 at 292
(retail order). Whole-unit byte diffs + far-away bl/jump-table targets
shift. A unit tuned against unit_check.py (pipeline-2 flags) will NOT link
exact if registered, and vice versa.

## House rule

Tune each unit under the SAME pipeline that will build it in the final
gate. Until GXInit.c/GXMisc.c are retuned for pipeline 1 they MUST stay
unregistered (they still compile via config.json auto-units, link exact,
gate green — just don't count in objdiff matched totals).

## Second trap: cherry-picking worktree commits can clobber orig/

sa-mtx2 accidentally committed the worktree's `orig` SYMLINK; cherry-pick
replaced main's real orig/ dir with a self-referential symlink (baserom
gone; every worktree symlink broke too). Recovery: rm symlink, recreate
orig/GFZE01/sys/, cp main.dol from ~/projects/fzero-gx-online/build/extract/
P-GFZE/sys/main.dol (sha1 verified 421c8810...b271). PRE-CHECK any
worktree-grown branch for an `orig` tree entry before picking:
`git ls-tree <sha> orig` must be a tree, not a symlink/blob.
