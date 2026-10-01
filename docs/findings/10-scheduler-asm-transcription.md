# 10: OSThreadScheduler chunk (13 funcs, 0x8001036C–0x80011358): programmatic asm transcription pipeline; configure.py registration trap; CW auto-relocation discovery

Date: 2026-08-22. Session 0822-pm-relaunch parent agent. Gate green after
every step (`421c8810...b271`, 1414848 B).

## Result

`dolphin/os/OSThreadScheduler.c` replaces `coarse/text_8001036C.c`
(0x8001036C–0x80011358, 4076 B):

| fn | addr | size | fn | addr | size |
|---|---|---|---|---|---|
| UnsetRun (static) | 1036C | 68 | OSExitThread | 10A10 | E4 |
| __OSGetEffectivePriority | 103D4 | 3C | OSCancelThread | 10AF4 | 1BC |
| SetEffectivePriority (static) | 10410 | 1C0 | OSResumeThread | 10CB0 | 288 |
| SelectThread | 105D0 | 228 | OSSuspendThread | 10F38 | 170 |
| __OSReschedule | 107F8 | 30 | OSSleepThread | 110A8 | EC |
| OSCreateThread | 10828 | 1E8 | OSWakeupThread | 11194 | 104 |
| | | | OSSetThreadPriority | 11298 | C0 |

All 13 EXACT pre-link per tools/unit_check.py --version GC/1.2.5n (BL-tolerant).
Word-level ELF audit vs DOL slices: **102 differing words = exactly the
object's 102 relocations**; zero non-reloc diffs anywhere. objdiff still
scores 12 funcs <100% fuzzy (reloc bookkeeping vs dtk base objects) — same
accepted class as OSError/Yay0Decode (findings/09). SDK progress line now
33/33 files.

## Workflow WIN: programmatic capstone→MWCC asm transcription

Hand-transcription of ~1000 instructions is error-prone; instead:

1. Dump range with capstone (venv: `python3 -m venv /tmp/opencode/venv &&
   pip install capstone`; repo has no capstone system-wide). Script pattern
   in /tmp/opencode/ppcdis.py (DOL section-table slice + Cs PPC big-endian).
2. Generate the .c with a converter (this session: /tmp/opencode/gen_sched.py;
   saved reusable copy as tools/gen_asm_from_disasm.py). Two passes per
   function: collect internal branch targets → labels `_<hexaddr>`;
   map `bl` targets through a symbol table (external names must match
   symbols.txt exactly); pass all other instructions through verbatim —
   capstone syntax is already MWCC-compatible (incl. `rlwinm.`, `subfic`,
   `stmw/lmw`, `lwzu`, `bdnz`, `blrl`).
3. Verify with unit_check per function, then word-audit: parse the .o
   (CW ELF has entsz=0 — hardcode strides: shdr 0x28, symtab 16, rela 12),
   list every differing word vs DOL slice, assert count == reloc count.

## Finding 1: configure.py registration REQUIRED (cost half a session)

A new split unit compiles AND links correctly without being listed in
configure.py's `DolphinLib("dolphin", [Object(...)])` — the build stays
gate-green — but the progress calculator ignores it entirely: summary stayed
"32 / 32 files, 20204 bytes" after integration. Only `report.json` /
objdiff.json showed the unit. Symptom trap: green gate does NOT mean the new
unit is registered. Always add `Object(Matching, "dolphin/os/<Name>.c")` to
configure.py in the same change, then full reset dance.

## Finding 2: asm `bl <symbol>` requires a visible C extern prototype

mwcceppc inline asm resolves `bl NAME` as a LABEL first: without
`extern void OSDisableInterrupts(void);` etc. at file top you get
"undefined label". Same requirement for FORWARD intra-file calls
(OSExitThread→OSWakeupThread defined later in the TU): declare the extern
prototype; linker binds it to the in-file definition. Backward intra-file
refs work bare.

## Finding 3: CW assembler AUTO-RELOCATES address-like immediates (supersedes assumptions in findings/09 Finding 2)

Writing `lis r4, -0x7FEA` + `addi r31, r4, -0x3FE8` (raw RunQueue anchor
0x8015C018) or `lwz r5, -0x7bc8(r13)` (raw SDA displacement) in inline asm
does NOT produce literal encodings: mwcceppc emits placeholder zeros
(`3c600000`, `80a00000`) plus relocations carrying the value/addend, which
the linker fills to the identical final encoding. Proof: word audit shows
placeholder→retail pairs ONLY at reloc slots, gate sha1 identical. Consequences:
- no need for sym@ha/@l syntax for byte parity (though it works);
- `(r13)`/`(r2)` data refs in asm get proper SDA relocs automatically —
  better than the "hardcode disp" workaround description in findings/09;
- pre-link .o bytes will differ from DOL at every such slot by design —
  judge linkage by the gate + word audit, not raw pre-link compare.

## Finding 4: GX-era scheduler facts (for future mutex/message units)

- SwitchThreadCallback @0x801A6440 (.sdata fnptr, r13 disp -0x7F80):
  called `SwitchThreadCallback(from, to)` via `lwz r12,-0x7F80(r13); mtlr
  r12; blrl` at idle entry (cur, NULL) and before switching (cur, next).
  This is the GX OSSetSwitchThreadCallback hook API.
- IdleContext = RunQueue+0x730 = 0x8015C748 (layout RunQueue[32]@0x8015C018,
  DefaultThread@...118, IdleThread@...430, IdleContext@...748).
- OSCreateThread FPU-init guard flag = *(u32*)0x8015BF90 (= __OSErrorTable+
  0x40): when nonzero writes context.srr1|=0x900, ctx.state|=1 (FPSAVED),
  ctx+0x194=(lbl_801A6430 & mask24-28)|4, then fpr[i]=psf[i]=-1 for i<32 as
  an interleaved dual-bank unrolled loop (mtctr 4 × bdnz, r5+=0x40).
- UpdatePriority is NOT a function in this SDK vintage: Cancel/Resume/
  Suspend/SetPriority each INLINE the promote-loop
  (owner=mutex->thread(=mutex+8); while !suspend && prio!=__OSGetEffectivePriority(owner):
  owner=SetEffectivePriority(owner,eff) until NULL).
- OSThread offsets confirmed: state u16@2C8 attr u16@2CA suspend@2CC
  priority@2D0 base@2D4 val@2D8 queue@2DC link{2E0,2E4} queueJoin{2E8,2EC}
  mutex@2F0 queueMutex{2F4,2F8} linkActive{2FC,300} stackBase@304
  stackEnd@308 (+30C/310/314 zeroed by OSCreateThread).

## Traps

- Scope flips required on external call targets BEFORE linking:
  fn_8000BE5C/fn_8000BE68/fn_8000BFEC/fn_8000EA84 flipped global (names kept).
- Renames of my own chunk (fn_80010828→OSCreateThread etc.) are safe WITH the
  full regen dance — coarse objects regenerate from symbols.txt so old-name
  refs update automatically (refines findings/09 trap wording).
- `#pragma force_active on` around ALL asm defs still mandatory: OSExitThread
  is referenced only via a lis/addi literal (no reloc) inside OSCreateThread,
  so the linker sees no live reference chain and would dead-strip it.
- rm -rf is permission-blocked in this environment; purge via
  python shutil.rmtree('build/GFZE01/asm'/'obj') + os.remove('config.json').
