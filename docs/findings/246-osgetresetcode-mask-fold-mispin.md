# 246 — OSGetResetCode cannot reach 100% under canonical flags (mask-fold mispin)

Date: 2026-08-28
Worker: hard (discriminate probe — bounded, one re-expression + one probe)
Unit: `main/dolphin/os/OSReset`
Symbol: `OSGetResetCode` (retail 0x30 B = 48 B, global)

## Retail disassembly (authoritative — from natc_loop --context-only)
```
0x00  lis r3, -0x8000
0x04  lbz r0, 0x30e2(r3)          ; *(u8*)0x800030E2
0x08  cmplwi r0, 0
0x0C  beq 0x18
0x10  lis r3, -0x8000
0x14  b 0x2c                      ; return 0x80000000
0x18  lis r3, -0x3400             ; PI base 0xCC003000
0x1C  addi r3, r3, 0x3000
0x20  lwz r0, 0x24(r3)            ; __PIRegs[9] (0xCC003024)
0x24  rlwinm r0, r0, 0, 0, 0x1c  ; & 0xFFFFFFF8  <-- KEY: mask kept as rlwinm
0x28  srwi r3, r0, 3              ; / 8
0x2C  blr
```
Reference body (dolsdk2001:src/os/OSReset.c:156) is `return (__PIRegs[9] & 0xFFFFFFF8) / 8;` — the dolsdk reference OMITTED the
GFZE01 guard `if (*(u8*)0x800030E2 != 0) return 0x80000000;` that retail performs. The m2c seed captured the guard correctly.

## What we tried (single complete function, one correct re-expression + one bounded probe)
v1 (flat addresses 0x800030E2 / 0xCC003024): 78.25
v2 (base+offset form 0x80000000[0x30e2] / [9]): 78.25 (identical — score invariant to addr form)
v3 (named base ptr (void*)0xCC003000 + 0x24): 78.25
v4 (split: v = pi[9]; v = v & 0xFFFFFFF8UL; return v>>3): 78.25
PROBE (v = (v<<0) & 0xFFFFFFF8UL): 78.25, disasm shows the rlwinm is STILL folded into srwi.

## Root cause (discriminate finding)
GC/1.2.5n at -O4,p algebraically folds `(x & 0xFFFFFFF8) >> 3` into a bare `srwi r3,r0,3`.
Retail emitted a separate `rlwinm r0,r0,0,0,0x1c` THEN `srwi`. No portable-C source rewrite
reproduces that exact instruction pair — MWCC always merges the constant mask into the shift.

## Verdict
- This is a COMPILER-FOLD mispin, NOT a class error. The logic is 100% correct.
- Candidate is a REGRESSION vs the existing 91.6% best (prior note "discriminate" is confirmed).
- Per the hard-worker contract: sub-100 candidates (and regressions) are NOT durably registered.
- Reaching 100% would require either (a) hand-asm / inline-asm for the masked read (T4/T20 — rejected
  by the natural-C gate), or (b) a compiler/scheduling permutation not available on this pin. Not the
  hard worker's job to brute-force. Hand to integ as a "matches-logic-not-schedule" note.

## Evidence artifacts
- v4 candidate: ~/.cache/natc/scratch/hard/OSReset_OSGetResetCode.c
- probe object: /home/armandofm/.cache/natc/scratch/87d836948c0d3000cedf9b875b0dc6b2.o (disasm = srwi-only)
- Authoritative compiler: GC/1.2.5n (sha ccf4b465...), flags from objdiff.json ninja edge.

## Symbol state
OSGetResetCode marked status=plateau (best remains 91.6% from an earlier context; 78.25 probe is not an improvement).
Lease NOT released: unit still has OSResetSystem + Reset in asm; this symbol is exhausted for portable-C context.
