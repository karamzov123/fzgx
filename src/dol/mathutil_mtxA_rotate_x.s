.include "macros.inc"
.file "mathutil_mtxA_rotate_x.c"

# 0x8006E324..0x8006E398 | size: 0x74
.text
.balign 4

# .text:0x0 | 0x8006E324 | size: 0x74
.fn mathutil_mtxA_rotate_x, global
/* 8006E324 0006B324  7C 88 02 A6 */	mflr r4
/* 8006E328 0006B328  4B FF EE 9D */	bl lbl_8006D1C4
/* 8006E32C 0006B32C  7C 88 03 A6 */	mtlr r4
/* 8006E330 0006B330  3C 80 E0 00 */	lis r4, 0xe000
/* 8006E334 0006B334  10 02 0C 20 */	ps_merge00 f0, f2, f1
/* 8006E338 0006B338  E0 64 00 04 */	psq_l f3, 0x4(r4), 0, qr0
/* 8006E33C 0006B33C  E0 84 00 14 */	psq_l f4, 0x14(r4), 0, qr0
/* 8006E340 0006B340  E0 A4 00 24 */	psq_l f5, 0x24(r4), 0, qr0
/* 8006E344 0006B344  10 C3 00 32 */	ps_mul f6, f3, f0
/* 8006E348 0006B348  10 E4 00 32 */	ps_mul f7, f4, f0
/* 8006E34C 0006B34C  11 05 00 32 */	ps_mul f8, f5, f0
/* 8006E350 0006B350  FC 20 08 50 */	fneg f1, f1
/* 8006E354 0006B354  10 C6 31 94 */	ps_sum0 f6, f6, f6, f6
/* 8006E358 0006B358  10 E7 39 D4 */	ps_sum0 f7, f7, f7, f7
/* 8006E35C 0006B35C  11 08 42 14 */	ps_sum0 f8, f8, f8, f8
/* 8006E360 0006B360  10 01 14 20 */	ps_merge00 f0, f1, f2
/* 8006E364 0006B364  D0 C4 00 04 */	stfs f6, 0x4(r4)
/* 8006E368 0006B368  10 C3 00 32 */	ps_mul f6, f3, f0
/* 8006E36C 0006B36C  D0 E4 00 14 */	stfs f7, 0x14(r4)
/* 8006E370 0006B370  10 E4 00 32 */	ps_mul f7, f4, f0
/* 8006E374 0006B374  D1 04 00 24 */	stfs f8, 0x24(r4)
/* 8006E378 0006B378  11 05 00 32 */	ps_mul f8, f5, f0
/* 8006E37C 0006B37C  10 C6 31 94 */	ps_sum0 f6, f6, f6, f6
/* 8006E380 0006B380  D0 C4 00 08 */	stfs f6, 0x8(r4)
/* 8006E384 0006B384  10 E7 39 D4 */	ps_sum0 f7, f7, f7, f7
/* 8006E388 0006B388  D0 E4 00 18 */	stfs f7, 0x18(r4)
/* 8006E38C 0006B38C  11 08 42 14 */	ps_sum0 f8, f8, f8, f8
/* 8006E390 0006B390  D1 04 00 28 */	stfs f8, 0x28(r4)
/* 8006E394 0006B394  4E 80 00 20 */	blr
.endfn mathutil_mtxA_rotate_x
