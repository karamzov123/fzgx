.include "macros.inc"
.file "func_80008378.c"

# 0x8006E5B4..0x8006E5FC | size: 0x48
.text
.balign 4

# .text:0x0 | 0x8006E5B4 | size: 0x48
.fn func_80008378, global
/* 8006E5B4 0006B5B4  10 00 0C 20 */	ps_merge00 f0, f0, f1
/* 8006E5B8 0006B5B8  10 22 1C 20 */	ps_merge00 f1, f2, f3
/* 8006E5BC 0006B5BC  48 00 00 0C */	b .L_8006E5C8
/* 8006E5C0 0006B5C0  E0 03 00 00 */	psq_l f0, 0x0(r3), 0, qr0
/* 8006E5C4 0006B5C4  E0 23 00 08 */	psq_l f1, 0x8(r3), 0, qr0
.L_8006E5C8:
/* 8006E5C8 0006B5C8  3C A0 E0 00 */	lis r5, 0xe000
/* 8006E5CC 0006B5CC  10 40 00 32 */	ps_mul f2, f0, f0
/* 8006E5D0 0006B5D0  10 41 10 7A */	ps_madd f2, f1, f1, f2
/* 8006E5D4 0006B5D4  10 42 10 94 */	ps_sum0 f2, f2, f2, f2
/* 8006E5D8 0006B5D8  C0 85 01 98 */	lfs f4, 0x198(r5)
/* 8006E5DC 0006B5DC  FC 02 20 00 */	fcmpu cr0, f2, f4
/* 8006E5E0 0006B5E0  41 80 00 10 */	blt .L_8006E5F0
/* 8006E5E4 0006B5E4  10 42 14 20 */	ps_merge00 f2, f2, f2
/* 8006E5E8 0006B5E8  10 00 00 B2 */	ps_mul f0, f0, f2
/* 8006E5EC 0006B5EC  10 21 00 B2 */	ps_mul f1, f1, f2
.L_8006E5F0:
/* 8006E5F0 0006B5F0  F0 03 00 00 */	psq_st f0, 0x0(r3), 0, qr0
/* 8006E5F4 0006B5F4  F0 23 00 08 */	psq_st f1, 0x8(r3), 0, qr0
/* 8006E5F8 0006B5F8  4E 80 00 20 */	blr
.endfn func_80008378
