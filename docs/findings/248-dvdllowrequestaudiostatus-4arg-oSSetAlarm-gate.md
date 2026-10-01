# 248 — DVDLowRequestAudioStatus: worker-layer BLOCKER (integrator-only OSSetAlarm 4-arg proto + codegen shaping)

**Unit:** `main/dolphin/dvd/dvdlow` (leased to natc3)
**Symbol:** `DVDLowRequestAudioStatus` @ 0x80016B6C, size 0x8C
**Provenance source:** `dolsdk2001:src/dvd/dvdlow.c:154` (also melee, melee-src-tmpcopy, mkdd, sms)

## Status
NOT exact-solvable as a worker under the current in-tree prototype. Verified this turn via one bounded
authoritative compile probe (`GC/1.2.5n`, head `449e9a21`).

## Evidence (this turn)
- `--context-only` relocation histogram for the function:
  - `R_PPC_ADDR16_HA` + `R_PPC_ADDR16_LO` ×2 → `AlarmForTimeout` (40 B .bss array), `AlarmHandlerForTimeout` (.text)
  - `R_PPC_EMB_SDA21` ×2 → `Callback` (.text fn-ptr), `StopAtNextInt` (.sbss scalar)
  - callees: `OSCreateAlarm` (1-arg), `OSSetAlarm` (called with **4 args**)
- Retail `OSSetAlarm` call ABI (from asm):
  `r3=&AlarmForTimeout, r4:r5=0x8000000000000000 (64-bit tick), r6=time, r7=&AlarmHandlerForTimeout`.
  **Handler is in r7** → a 4-argument call.
- In-tree prototype `dvdlow.c:43`: `extern void OSSetAlarm(OSAlarm* alarm, s64 tick, OSAlarmHandler handler);`
  is **3-arg** → handler would land in r6, not r7. Mismatch = the "dvdlow OSSetAlarm 4-arg (r7), integrator-only" gotcha.

## Probe results
1. Faithful candidate with the in-tree 3-arg proto: historical best = **90.914%** (ledger, 19 attempts) — handler lands r6, never 100%.
2. This turn, candidate transcribed with the **retail-accurate 4-arg proto**
   (`OSSetAlarm(OSAlarm*, s64 tick, u32 time, OSAlarmHandler handler)`) to test solvability:
   - Compiles cleanly (confirms proto direction is correct; no arg-count error).
   - `Callback = arg1` needed `(DVDCallback)` cast (Callback is `extern DVDCallback`).
   - **Score = 74.057%** (worse than the 3-arg attempt) — MMIO base recomputed per-store and `arg0|0xE2000000`
     sequencing drift the codegen. So the 4-arg proto is necessary-but-not-sufficient; body also needs phase 9-12
     codegen shaping (likely hoist the `0xCC006000` base into a register like retail, match store ordering).

## Conclusion / required unblock
This function is a **concrete blocker**, not a worker retry:
- **Integrator-only:** land the 4-arg `OSSetAlarm` prototype (handler in r7) in `dvdlow.c` + headers. Worker must NOT alter the prototype.
- **Worker (post-proto):** re-shape the body so the compiler emits r6=time, r7=handler and reuses the `0xCC006000` base
  register; target 100% with exact `R_PPC_EMB_SDA21`/`R_PPC_ADDR16` relocations.
- No durable registration this turn (max 74.057%, below 100% gate). Not a duplicate of `__DVDLowSetWAType` (which is
  separately terminal: authoritative rescore 99.235, `duplicate_of` against head 449e9a21).

## Files (scratch, not committed)
- candidate body: `/home/armandofm/.cache/natc/scratch/natc3/DVDLowRequestAudioStatus.body.c`
- spliced whole-file candidate: `/home/armandofm/.cache/natc/scratch/natc3/dvdlow.audstat.cand.c`
- bounded 4-arg probe TU: `/home/armandofm/.cache/natc/scratch/natc3/dvdlow.audstat.probe.c`
