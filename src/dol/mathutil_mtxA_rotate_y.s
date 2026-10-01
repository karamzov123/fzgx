.include "macros.inc"
.file "mathutil_mtxA_rotate_y.c"

# 0x8006E398..0x8006E424 | size: 0x8C
.text
.balign 4

# .text:0x0 | 0x8006E398 | size: 0x8C
.fn mathutil_mtxA_rotate_y, global
/* 8006E398 0006B398  7C 88 02 A6 */	mflr r4
/* 8006E39C 0006B39C  4B FF EE 29 */	bl lbl_8006D1C4
/* 8006E3A0 0006B3A0  7C 88 03 A6 */	mtlr r4
/* 8006E3A4 0006B3A4  3C 80 E0 00 */	lis r4, 0xe000
/* 8006E3A8 0006B3A8  10 01 14 20 */	ps_merge00 f0, f1, f2
/* 8006E3AC 0006B3AC  C0 64 00 00 */	lfs f3, 0x0(r4)
/* 8006E3B0 0006B3B0  C0 C4 00 08 */	lfs f6, 0x8(r4)
/* 8006E3B4 0006B3B4  C0 84 00 10 */	lfs f4, 0x10(r4)
/* 8006E3B8 0006B3B8  C0 E4 00 18 */	lfs f7, 0x18(r4)
/* 8006E3BC 0006B3BC  C0 A4 00 20 */	lfs f5, 0x20(r4)
/* 8006E3C0 0006B3C0  C1 04 00 28 */	lfs f8, 0x28(r4)
/* 8006E3C4 0006B3C4  10 63 34 20 */	ps_merge00 f3, f3, f6
/* 8006E3C8 0006B3C8  10 84 3C 20 */	ps_merge00 f4, f4, f7
/* 8006E3CC 0006B3CC  10 A5 44 20 */	ps_merge00 f5, f5, f8
/* 8006E3D0 0006B3D0  10 C3 00 32 */	ps_mul f6, f3, f0
/* 8006E3D4 0006B3D4  10 E4 00 32 */	ps_mul f7, f4, f0
/* 8006E3D8 0006B3D8  11 05 00 32 */	ps_mul f8, f5, f0
/* 8006E3DC 0006B3DC  FC 20 08 50 */	fneg f1, f1
/* 8006E3E0 0006B3E0  10 C6 31 94 */	ps_sum0 f6, f6, f6, f6
/* 8006E3E4 0006B3E4  10 E7 39 D4 */	ps_sum0 f7, f7, f7, f7
/* 8006E3E8 0006B3E8  11 08 42 14 */	ps_sum0 f8, f8, f8, f8
/* 8006E3EC 0006B3EC  10 02 0C 20 */	ps_merge00 f0, f2, f1
/* 8006E3F0 0006B3F0  D0 C4 00 08 */	stfs f6, 0x8(r4)
/* 8006E3F4 0006B3F4  10 C3 00 32 */	ps_mul f6, f3, f0
/* 8006E3F8 0006B3F8  D0 E4 00 18 */	stfs f7, 0x18(r4)
/* 8006E3FC 0006B3FC  10 E4 00 32 */	ps_mul f7, f4, f0
/* 8006E400 0006B400  D1 04 00 28 */	stfs f8, 0x28(r4)
/* 8006E404 0006B404  11 05 00 32 */	ps_mul f8, f5, f0
/* 8006E408 0006B408  10 C6 31 94 */	ps_sum0 f6, f6, f6, f6
/* 8006E40C 0006B40C  D0 C4 00 00 */	stfs f6, 0x0(r4)
/* 8006E410 0006B410  10 E7 39 D4 */	ps_sum0 f7, f7, f7, f7
/* 8006E414 0006B414  D0 E4 00 10 */	stfs f7, 0x10(r4)
/* 8006E418 0006B418  11 08 42 14 */	ps_sum0 f8, f8, f8, f8
/* 8006E41C 0006B41C  D1 04 00 20 */	stfs f8, 0x20(r4)
/* 8006E420 0006B420  4E 80 00 20 */	blr
.endfn mathutil_mtxA_rotate_y
