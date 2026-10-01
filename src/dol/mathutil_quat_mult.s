.include "macros.inc"
.file "mathutil_quat_mult.c"

# 0x8006E540..0x8006E5B4 | size: 0x74
.text
.balign 4

# .text:0x0 | 0x8006E540 | size: 0x74
.fn mathutil_quat_mult, global
/* 8006E540 0006B540  C0 84 00 00 */	lfs f4, 0x0(r4)
/* 8006E544 0006B544  C0 A4 00 04 */	lfs f5, 0x4(r4)
/* 8006E548 0006B548  C0 C4 00 08 */	lfs f6, 0x8(r4)
/* 8006E54C 0006B54C  C0 E4 00 0C */	lfs f7, 0xc(r4)
/* 8006E550 0006B550  C1 05 00 00 */	lfs f8, 0x0(r5)
/* 8006E554 0006B554  C1 25 00 04 */	lfs f9, 0x4(r5)
/* 8006E558 0006B558  C1 45 00 08 */	lfs f10, 0x8(r5)
/* 8006E55C 0006B55C  C1 65 00 0C */	lfs f11, 0xc(r5)
/* 8006E560 0006B560  FC 06 02 72 */	fmul f0, f6, f9
/* 8006E564 0006B564  FC 24 02 B2 */	fmul f1, f4, f10
/* 8006E568 0006B568  FC 45 02 32 */	fmul f2, f5, f8
/* 8006E56C 0006B56C  FC 66 02 B2 */	fmul f3, f6, f10
/* 8006E570 0006B570  EC 05 02 B8 */	fmsubs f0, f5, f10, f0
/* 8006E574 0006B574  EC 26 0A 38 */	fmsubs f1, f6, f8, f1
/* 8006E578 0006B578  EC 44 12 78 */	fmsubs f2, f4, f9, f2
/* 8006E57C 0006B57C  EC 65 1A 7A */	fmadds f3, f5, f9, f3
/* 8006E580 0006B580  EC 04 02 FA */	fmadds f0, f4, f11, f0
/* 8006E584 0006B584  EC 25 0A FA */	fmadds f1, f5, f11, f1
/* 8006E588 0006B588  EC 46 12 FA */	fmadds f2, f6, f11, f2
/* 8006E58C 0006B58C  EC 64 1A 3A */	fmadds f3, f4, f8, f3
/* 8006E590 0006B590  EC 07 02 3A */	fmadds f0, f7, f8, f0
/* 8006E594 0006B594  EC 27 0A 7A */	fmadds f1, f7, f9, f1
/* 8006E598 0006B598  EC 47 12 BA */	fmadds f2, f7, f10, f2
/* 8006E59C 0006B59C  EC 67 1A F8 */	fmsubs f3, f7, f11, f3
/* 8006E5A0 0006B5A0  D0 03 00 00 */	stfs f0, 0x0(r3)
/* 8006E5A4 0006B5A4  D0 23 00 04 */	stfs f1, 0x4(r3)
/* 8006E5A8 0006B5A8  D0 43 00 08 */	stfs f2, 0x8(r3)
/* 8006E5AC 0006B5AC  D0 63 00 0C */	stfs f3, 0xc(r3)
/* 8006E5B0 0006B5B0  4E 80 00 20 */	blr
.endfn mathutil_quat_mult
