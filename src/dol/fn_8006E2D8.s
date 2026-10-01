.include "macros.inc"
.file "fn_8006E2D8.c"

# 0x8006E2D8..0x8006E324 | size: 0x4C
.text
.balign 4

# .text:0x0 | 0x8006E2D8 | size: 0x4C
.fn fn_8006E2D8, global
/* 8006E2D8 0006B2D8  E0 85 00 00 */	psq_l f4, 0x0(r5), 0, qr0
/* 8006E2DC 0006B2DC  C0 A5 00 08 */	lfs f5, 0x8(r5)
/* 8006E2E0 0006B2E0  10 84 00 72 */	ps_mul f4, f4, f1
/* 8006E2E4 0006B2E4  E0 C5 00 10 */	psq_l f6, 0x10(r5), 0, qr0
/* 8006E2E8 0006B2E8  EC A5 00 72 */	fmuls f5, f5, f1
/* 8006E2EC 0006B2EC  C0 E5 00 18 */	lfs f7, 0x18(r5)
/* 8006E2F0 0006B2F0  10 86 20 BA */	ps_madd f4, f6, f2, f4
/* 8006E2F4 0006B2F4  E1 05 00 20 */	psq_l f8, 0x20(r5), 0, qr0
/* 8006E2F8 0006B2F8  EC A7 28 BA */	fmadds f5, f7, f2, f5
/* 8006E2FC 0006B2FC  C1 25 00 28 */	lfs f9, 0x28(r5)
/* 8006E300 0006B300  41 82 00 10 */	beq .L_8006E310
/* 8006E304 0006B304  10 88 20 FA */	ps_madd f4, f8, f3, f4
/* 8006E308 0006B308  EC A9 28 FA */	fmadds f5, f9, f3, f5
/* 8006E30C 0006B30C  48 00 00 0C */	b .L_8006E318
.L_8006E310:
/* 8006E310 0006B310  10 88 20 FE */	ps_nmadd f4, f8, f3, f4
/* 8006E314 0006B314  EC A9 28 FE */	fnmadds f5, f9, f3, f5
.L_8006E318:
/* 8006E318 0006B318  F0 84 00 00 */	psq_st f4, 0x0(r4), 0, qr0
/* 8006E31C 0006B31C  D0 A4 00 08 */	stfs f5, 0x8(r4)
/* 8006E320 0006B320  4E 80 00 20 */	blr
.endfn fn_8006E2D8
