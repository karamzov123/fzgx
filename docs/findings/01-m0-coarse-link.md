# M0 asm-only link: GC/1.3 requires coarse split units

2026-08-21, session 2. Supersedes the linker-blocker conclusions in
`00-toolchain-bringup.md` and the Session 1 addendum in `docs/PROGRESS.md`.

## Result

M0 is green. The normal Ninja build reproduces the retail DOL byte-for-byte:

```text
$ ninja -v build/GFZE01/main.dol
[1/4] build/tools/wibo build/compilers/GC/1.3/mwldeppc.exe ...
[2/4] build/tools/dtk elf2dol build/GFZE01/main.elf build/GFZE01/main.dol

$ sha1sum build/GFZE01/main.dol orig/GFZE01/sys/main.dol
421c88106697d3275a3fc26fb7a01bf6d816b271  build/GFZE01/main.dol
421c88106697d3275a3fc26fb7a01bf6d816b271  orig/GFZE01/sys/main.dol
$ cmp -s build/GFZE01/main.dol orig/GFZE01/sys/main.dol; echo $?
0
```

Both files are 1,414,848 bytes.

## Root cause

The blocker was object granularity, not pipe buffering and not a permanently
broken linker wrapper:

- 2,216 one-function units: GC/1.3 under wibo produced no output and exceeded
  120–180 second timeouts.
- 35 coarse units (approximately 0x8000 bytes, boundaries advanced so they do
  not cut analyzed symbols): the same GC/1.3 linker under the same wibo 1.0.3
  completed within the command timeout and produced a valid 1,752,848-byte
  ELF.
- GC/2.7 linked the 2,216-unit input quickly, but its DOL differed in only five
  regions: four startup copy/clear-table size bytes and the `.dtors` function
  pointer. This is linker-version behavior, not a bad section model. The DOL
  header and size already matched exactly.
- GC/1.3 with the coarse layout emits the retail startup metadata exactly.

The earlier 11-object failures used the obsolete section model and do not
contradict this result.

## Implementation

`tools/gen_initial_splits.py` now emits the reproducible M0 coarse baseline.
It derives section ends from analyzed symbol extents, proposes 0x8000-byte
boundaries, and moves any boundary that falls inside a symbol to the next
valid aligned address. Oversized opaque data symbols stay whole. As source is
matched, these coarse assembly units will be replaced by real source-file
units.

## Remaining warnings

`dtk dol split` still reports the known analyzer warnings around
0x8006D000–0x8006E200 and the unresolved extab/extabindex candidate at
0x8008FEE4. They do not affect the byte-identical M0 output and remain analysis
work for later milestones.
