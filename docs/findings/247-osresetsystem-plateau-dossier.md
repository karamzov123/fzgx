# 247 — OSResetSystem (648B) plateau: struct fix reused, residual = T10 tail scheduling

Date: 2026-08-28
Worker: hard (discriminate — bounded; information not volume)
Unit: `main/dolphin/os/OSReset`  Symbol: `OSResetSystem`  (648 B, R_PPC_EMB_SDA21 x1)
Pin: GC/1.2.5n (authoritative, from objdiff.json scratch)
Verified result: candidate src_sha `5c137beccf918a7a` → **diff_score 68.21** (recorded attempt;
  later identical resubmits return `duplicate_candidate` — correctly refused, not retried).

## WHAT THIS CONFIRMS
- findings/245 struct reconstruction is MANDATORY and REUSED here: declaring
  `ResetFunctionQueue_801A67D0` as `struct {head,tail}` makes MWCC emit the single
  `R_PPC_EMB_SDA21` load retail has (`lwz 27,0(0)` then `lwz 27,8(r27)`). Declaring it
  `unsigned char[8]` / `long long` / `void*` yields the wrong access form.
- The retail function INLINES `CallResetFunctions(phase)` as two queue-walk loops:
  walk `head`, call `info->func(phase)`, accumulate `rc != 0` via `cntlzw r0,r3; srwi r0,r0,5`
  (the "boolean to 0/1" idiom), `or` into a running flag, then OR `__OSSyncSram() != 0`.
  The dolsdk reference body calls `CallResetFunctions` directly — reference INCOMPLETE,
  which is exactly the "discriminate" note. Inlining the queue walk reproduces the
  `cntlzw/srwi/or` reduction retail shows.

## DISASSEMBLY MATCH (candidate obj ec32bc10…, 648 B)
Instruction-for-instruction the control flow matches retail:
- sda21 queue load (`lwz 27,0(0)`), `li 28,0`, the two queue-walk loops with
  `mtlr; blrl; cntlzw; srwi 5; or 28,28,0` flag reduction, `cmplwi 27,0` loop guards.
- SRAM flag-set: `lbz 0,19(r3); ori 0,0,0x40; stb 0,19(r3)` (the `flags |= 0x40`).
- PI reset: `lis 3,-13312; addi 3,3,8192; li 0,0; sth 0,2(r3)` → `*(u16*)0xCC003002 = 0`.
- `slwi 3,29,3` = `resetCode*8` into `Reset`, and the OS-reboot unwinding tail
  (`lwz 3,220(r3)` / `lhz` / `lwz 27,764(r3)` thread-cancel region).

## THE RESIDUAL ~32% (NOT class errors — T10 scheduling)
68% = the body is structurally right but the 648-byte tail diverges in instruction
ORDER / register allocation from retail, in:
1. The `rc`/`flag` liveness through the `recal` variable and the phase-0 vs phase-1
   loop separation (retail keeps `r28`/`r27` as two separate accumulators; my source
   reuses one `flag` which schedules differently).
2. The trailing OS-reboot block (`OSCancelThread` + `memset`-style teardown, the
   `th = *(void**)0xCC002FFC` / `lhz 0,712(r3); lwz 27,764(r3)` region) — exact
   statement ordering needed to pin retail's tail.

This is a multi-point codegen plateau: closing it requires iterative scheduling
shaping of a 648-byte body (statement reordering, accumulator splitting) that is
integrator-domain work in canonical `src/`, not a single portable-C re-expression a
worker can land blind.

## RECOMMENDATION (for integ, highest-value handoff)
1. Land findings/245 struct type into `src/dolphin/os/OSReset.c` (prerequisite for
   BOTH `OSRegisterResetFunction` and `OSResetSystem` sda21 correctness).
2. In canonical src, write `OSResetSystem` with the inlined `CallResetFunctions`
   queue-walk (flag idiom `cntlzw>>5`) and iterate scheduling on the tail using the
   recorded 68.21% candidate (`~/.cache/natc/scratch/hard/OSReset_OSResetSystem.c`)
   as the structural baseline. Expect ~3–5 scheduling passes to reach 100%.
3. Do NOT re-run the worker candidate as-is: it is a `duplicate_candidate` (already
   scored 68.21).

## ARTIFACTS
- Candidate (verified 68.21): ~/.cache/natc/scratch/hard/OSReset_OSResetSystem.c
- Recorded obj: ~/.cache/natc/scratch/ec32bc1011a28d226fa36c64219a64ab.o
- Hypothesis bundle: ~/.cache/natc/hypotheses/main_dolphin_os_OSReset/OSResetSystem.json
