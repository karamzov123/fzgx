# 14: OSMutex family absent-but-one; OSAlloc ctx-swap allocator mapped; r13 base correction; two tooling traps

Date: 2026-08-22. Session 0822-pm2 (post-relaunch). Two units matched and
committed: dolphin/os/OSMutex.c (112 B, natural C, 100%) and
dolphin/os/OSAlloc.c (2440 B, asm transcription, linked-exact). Gate green
throughout.

## Finding 1: retail GX contains exactly ONE OSMutex.c function

Handoff §2 guessed "OSMutex.c-shaped funcs around fn_8000EA84". WRONG in a
useful way: whole-DOL caller scans show

- zero callers of `__OSGetEffectivePriority` outside the scheduler TU
  (OSCancelThread/OSResumeThread/OSSuspendThread/OSSetThreadPriority only);
- zero OSLockMutex-shaped functions among the six OSSleepThread callers
  (all are DVD/VI/game/GXDrawDone sites).

Since an inlined promote-loop would have to call one of those, OSLockMutex /
OSUnlockMutex / OSWaitCond / OSTryLockMutex / OSInitMutex were all
DEAD-STRIPPED by the linker (never referenced by any linked object). Only
`__OSUnlockAllMutex` survives, pulled in by OSExitThread/OSCancelThread
(OSThreadScheduler.o bls it twice). Unit = that single function, natural C,
byte-exact except its own bl slot. Do not hunt for the rest of the family.

## Finding 2: OSAlloc family map (GX custom "context swap" allocator)

Cross-pollinated from fzero-gx-online findings/10–12 (verified call sites):

| addr | name | note |
|---|---|---|
| 0x80008DB4 | ? | not alloc (calls 6Exxx/15B78) |
| 0x80008EC8 | OSInitAlloc | maxHeaps=8; calls static array-init fn_80009468 (already in OSAllocCtx.c) |
| 0x80008F60/F88 | wrappers | both -> OSCreateHeap impl |
| 0x80008FB0 | OSCreateHeap | online-verified |
| 0x80008E84 | OSSetCurrentHeap | online-verified; = save(951C)/set curr/restore(95A4) |
| 0x80008E34 / E5C | OSAlloc/OSFree-style thunks | pass (&lbl_801A6414, 0) to impls |
| 0x80009830 | OSAllocFromHeap | online-verified; now renamed |
| 0x80009AA8 | (free impl) | almost surely OSFreeToHeap — rename after confirming |
| 0x800090A4 | check/dump | huge, ~20× OSReport + save/restore pairs |
| 0x80009064 | ? | save/restore pair, small |
| 0x8000961C/9CAC/9CD4 | ? | transcribed asm, semantics TBD |

GX diverges from melee OSAlloc.c: HeapDesc stride is **0x14** (5 words:
size/free/allocated + 2 extra), HeapArray is a FIXED bss array
lbl_8015BE40 (8×0x14), and heap switches SAVE/RESTORE five globals
(-0x7FB0 curr, -0x7C7C, -0x7C80 NumHeaps?, -0x7C84/-0x7C88 arena bounds)
via fn_8000951C/fn_800095A4 (matched in OSAllocCtx.c). Melee has none of
this — do not port melee C blindly.

## Finding 3: r13/sdata base CORRECTION (supersedes findings/11 value)

findings/11 wrote r13 base = 0x8019E3C0 — digit typo. Correct base is
**0x801AE3C0**, confirmed four ways: OSExceptionTable 0x801A676C @ -0x7C54;
SwitchThreadCallback 0x801A6440 @ -0x7F80; HeapArray ptr lbl_801A6744 @
-0x7C7C; __OSCurrHeap lbl_801A6410 @ -0x7FB0 (matches online's "r13-32688").
r2 = 0x800FEE40 unchanged. (findings/10-Finding-3 auto-reloc still applies:
raw `(r13)` displacements in inline asm become SDA relocs automatically.)

## Finding 4: `configure.py <mode>` positional arg poisons build.ninja

Running `python3 configure.py configure` writes `configure_args =
configure` into build.ninja (tools/project.py:470 records sys.argv[1:]).
The progress rule then runs `configure.py configure progress` → argparse
error on EVERY build. Always invoke as `python3 configure.py -v GFZE01`.

## Traps (repeat offenders)

- Forward `bl` between asm bodies still needs a file-top extern prototype
  (fn_80009CAC→fn_80009CD4; findings/10 F2 again).
- Renaming a symbol in the SOURCE but not in symbols.txt breaks coarse-unit
  refs at link ("undefined: 'fn_80009830'"); rename BOTH or neither —
  coarse objects regenerate from symbols.txt so old-name refs follow it.
- MWCC without headers has no NULL — use 0 in natural C units.
- ppcdis consumers must parse symbols.txt hex case-insensitively ([0-9a-fA-F]);
  addresses in the file are uppercase.
