# 21: region [0x8007A060–0x8007FF18] fully carved as msl/stdio_8007A060.c — MSL stdio/file I/O block, gate green

Date: 2026-08-23. pm9-c subagent carver. Coarse/text_8007A060.c ELIMINATED.

## Result

- 46 funcs / 24248 B transcribed to `src/dolphin/msl/stdio_8007A060.c`
  (generator: `tools/gen_stdio_carve.py`, adapted from gen_gamehead.py).
- Gate: sha1 `421c8810...b271`, dol byte-identical (`cmp` clean), 173 files,
  SDK 790/1886 funcs. Pre-link word audit: only diff class is bl slots
  (48000001 placeholders) — expected pre-link.
- configure.py + splits.txt updated; committed on
  hermes-subagent/subagent-sa-2-202a9f8c.

## Classification

MSL stdio/file-I/O block (not MTX): exit/atexit machinery
(fn_8007A150/A1C0 = __stdio-atexit lazy init around static file table at
lbl_801A3380), scanf family (fn_8007B338 4224B dispatches %d/%i/%u/%o/%x/%n/
%e/%f/%g via fn_80087E58/80088128/80088598), printf core fn_8007CF18 (6020B),
__fwrite/fwrite/fseek/fget-family, __flush_buffer/__prep_buffer, and the
3-instruction __begin/__end/__kill_critical_region stubs (all plain `blr` —
retail built with interrupt suppression compiled out).

## KEY TRAP: exit() must reference _dtors by SYMBOL

With this unit linked, GC/1.3 aborted with "Destructors must be called in
'exit'". Cause: retail exit() materializes &_dtors (0x8008FF20) with
lis/addi; writing those as numeric constants compiles fine and links to the
same bytes, but the LINKER only sees a .dtors reference via relocs, so it
thinks no TU calls destructors. Fix: declare `extern unsigned char _dtors[]`
and emit `lis r3, _dtors@ha` / `addi r0, r3, _dtors@l`. Bytes identical;
reloc satisfies the check. Any future carve of a TU containing exit() needs
this.

## Other traps

- Worktree baserom was missing again (orig/GFZE01/sys/main.dol) — restore
  from fzero-gx-native extracted dir before anything else.
- Subagent worktrees lag mainline: merge latest parent branch first or you
  rebuild against stale splits (here: missing CARDCreate.c broke the build
  until cherry-picked).
- build.ninja's recorded `configure_args = configure` poisons the progress
  step (findings/14 F4); sed it to empty before running ninja.
