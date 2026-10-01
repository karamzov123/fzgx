# 07: mwldeppc dead-strips unreferenced globals; SDA21 relocs normalize at link

Date: 2026-08-22. Session 6 (PM relaunch). Finishes WIP commit 13a0d4d
(OSThread.c/OSThreadTable.c).

## Result

Gate restored to sha1 421c8810...b271 after both units landed:
- dolphin/os/OSThreadTable.c @0x80009FA4–0x80009FBC (2 funcs, natural C).
- dolphin/os/OSThread.c @0x8001029C–0x8001036C (5 funcs; OSInitThreadQueue,
  fn_800102AC, fn_800102B8 EXACT pre-link; OSDisableScheduler/OSEnableScheduler
  exact post-link). SDK Code: 9/9 files, 58/61 funcs, 1556/1720B,
  **100.00% linked**.

## Finding 1: mwldeppc dead-strips unreferenced GLOBAL functions

Symptoms when OSThreadTable.o entered the link: DOL came out 96B SHORT of
retail (1414752 vs 1414848), PPCMfmsr slid -0x18 into fn_80009FA4's slot,
every absolute data literal (@ha/@l pairs) downstream changed value, and
fn_80009FA4/fn_80009FB4 were absent from main.elf despite being GLOBAL and
listed in ldscript objects. Same later for fn_800102B8 (52B, delta step
-24→-76). Root cause: no object in the current partial link references them
(no bl anywhere in generated asm), and GC/1.x mwldeppc strips unreachable
code even for global syms.

Fix (melee house pattern, e.g. melee gmopening.c:90):
```c
#pragma push
#pragma force_active on
... function ...
#pragma pop
```
Applied to all three unreferenced functions → bytes retained, gate green.
Any future unit whose functions nobody calls yet needs this pragma.
Diagnosis workflow that found it (reusable):
1. Compare DOL headers section-by-section (here: .text -96B only).
2. Delta profile: readelf -s on main.elf vs symbols.txt addresses, keyed by
   NAME, print where act-exp changes → shows exact drop boundaries
   (-24 at OSThreadTable, -76 after fn_800102B8).
3. grep generated asm for refs to confirm zero callers.

## Finding 2: SDA21 relocs normalize AT LINK — pre-link DIFF is expected

unit_check showed OSDisableScheduler/OSEnableScheduler "DIFF" on exactly the
Reschedule words: our object encodes `lwz r4,0(r0)` (rA=0!) with R_PPC_EMB_SDA21
relocs at insn+2, retail has `lwz r4,sda21(r13)`. After linking, mwldeppc
resolves the reloc to the true r13 displacement (-0x7BC0 etc.) AND rewrites the
base-register field to r13 — linked bytes EXACT. Same story for
fn_80009FA4/lbl_801A6744. Consequence: for extern-global accesses, trust the
sha1/objdiff-linked verdict over unit_check word diffs; sda21-form mismatches
pre-link are NOT real diffs if the reloc exists (readelf -r confirms).

## Traps

- The red-gate diff starts EARLY (first `bl` displacement at 0x800055ec) because
  branch targets and data literals shift — don't chase the first mismatch;
  find the LAYOUT hole first.
- zsh eats bare `===` echo separators (runs as command); quote them.
- ninja tail hides completed edges: a FAILED shasum stamp can still mean the
  link itself ran fine — check artifact mtimes before assuming staleness.
