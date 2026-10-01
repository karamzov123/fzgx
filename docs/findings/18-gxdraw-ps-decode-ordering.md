# 18: GXDraw carve: capstone PS-arithmetic misdecodes; MWCC ps operand order; asm def ordering

Date: 2026-08-22. w4-gxgeom agent, batches 7-10 (GXLight + GXDraw
0x800372E0-0x8003BB30, ~18KB total this session). All EXACT post-link.

## Finding 1: blocker for batch 7 was a stale duplicate split block

splits.txt had BOTH `GXGeometry.c: 0x800372E0-0x8003887C` and the new
`GXLight.c` block at the same range → "Split overlaps with previous split".
Always grep splits.txt for the new range before carving.

## Finding 2: capstone decodes Gekko PS arithmetic as AltiVec/VSX

Not just psq_st (findings/17): ALL opcode-4 arithmetic decodes garbage —
ps_nmsub→"vmsumshm"/"vaddeuqm", ps_madds0→"vmhaddshs", psq_stux→"xxsldwi",
D-form psq→"xsaddsp". UNDEC lines AND any vm*/vs*/xs*/xx*/lx* mnemonic in an
op4 region must be re-decoded from raw DOL words:
- fcmpu/o: op63, xo=(w>>1)&1023 in {0,32}
- PS arith: op4, xo5=(w>>1)&31: 12 muls0,13 muls1,14 madds0,15 madds1,
  18 div,20 sub,21 add,22?,23 sel,24 res,25 mul,26 rsqrte,29 madd,
  28 msub,30 nmsub,31 nmadd; merges by xo10: 528 merge00,560 merge01,
  592 merge10,624 merge11
- psq D-form: op 56/57/60/61 = psq_l/lu/st/stu, S,A,W(10),I(7),d
- psq indexed: xo10 6 lx,38 lux,7 stx,39 stux

## Finding 3: MWCC ps_madd-family operand order is fD,fA,fC,fB

The ADDEND goes LAST. Emitting hardware order D,A,B,C encodes wrong words.
Correct emission from word fields: `{mn} fD, fA, fC, fB` for
madd/msub/nmadd/nmsub/madds0/madds1. Verified by encode round-trip.

## Finding 4: source order of asm defs must match retail layout

CW emits functions to the .o in SOURCE ORDER. After patch-surgery scrambled
order inside GXDraw.c, every function after the first divergence shifted;
the linked dol looked like garbage at expected vaddrs. Fix: keep defs sorted
by start address within a unit.

## Traps

- Split range must COVER the whole carved region or dtk auto-creates a unit
  for the gap → multiply-defined symbols at link.
- Subagent worktrees may commit `orig` SYMLINK (again): pre-check
  `git ls-tree <sha> orig`; restore baserom from online extract if clobbered.
- Forward decls: declare ALL later-defined asm callees before first use.
