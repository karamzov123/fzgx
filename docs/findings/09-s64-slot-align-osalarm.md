# 09: GX MWCC s64 ABI + OSAlarm unit traps

Date: 2026-08-22. Proven on dolphin/os/OSAlarm.c (8 funcs, 1836B,
all linked-exact, gate green).

## Finding 1: s64 args skip to even slot counting r3 = slot 0

Retail `OSSetAlarm(OSAlarm*, OSTime tick, handler)` receives tick in
**r5:r6** with r4 as dead pad (retail SIBios caller confirmed: r4 unset
before call). Rule: 64-bit params align to even slot index where r3=slot0;
after a pointer param (slot0), next even is slot2 → r5:r6. Chained:
`OSSetPeriodicAlarm(alarm, start r5:r6, period r7:r8, handler r9)`.
Natural C with plain `s64` prototypes reproduces this under GC/1.2.5n —
no dummy params needed. First-param s64 (e.g. __OSTimeToSystemTime) stays
r3:r4.

## Traps (each cost a build cycle)

1. **SetTimer helper**: static function → standalone body survives
   (-inline auto won't inline multi-call-site); macro form changes register
   allocation at call sites (regalloc differs from call-form). Winning
   form: `static __inline void f(...)` — fully inlined AND regalloc
   identical to the static-fn-with-bl variant.
2. **Defined sbss statics shift .sdata2**: `static struct {...} AlarmQueue;`
   appended an 8B object to .sbss, pushing sbss end across a 32-boundary →
   .sdata2 start slid +0x20, every late-section literal diffed. Fix: declare
   `extern` only + flip the symbols.txt entry to scope:global (ldscript
   places it absolutely). Same pattern sa-gx used for CPUFifo/GPFifo.
3. **OSContext local alignment**: retail keeps exceptionContext at r1+0x18
   (8-aligned); u32-array typedef put mine at r1+0x14. Fix: u64 fields.
4. **__div2i**: mwcceppc emits runtime helper name `__div2i` for s64/s64
   division; renamed symbols.txt fn_800799DC→__div2i (0x800799DC) and sed'd
   twin's SIBios/EXIBios/OSTimeCal callers in same commit.

## Status after unit

SDK Code 16348/24544B, 142 funcs, 18 files, 100% linked. Gate green.
