.include "macros.inc"
.file "fn_8003BCE4.c"

# 0x8003BCE4..0x8003BD78 | size: 0x94
.text
.balign 4

# .text:0x0 | 0x8003BCE4 | size: 0x94
.fn fn_8003BCE4, global
/* 8003BCE4 00038CE4  C0 02 83 64 */	lfs f0, lbl_801A71A4@sda21(r0)
/* 8003BCE8 00038CE8  C0 22 83 68 */	lfs f1, lbl_801A71A8@sda21(r0)
/* 8003BCEC 00038CEC  C0 42 83 6C */	lfs f2, lbl_801A71AC@sda21(r0)
/* 8003BCF0 00038CF0  C0 62 83 70 */	lfs f3, lbl_801A71B0@sda21(r0)
/* 8003BCF4 00038CF4  C0 82 83 74 */	lfs f4, lbl_801A71B4@sda21(r0)
/* 8003BCF8 00038CF8  E0 A5 20 00 */	psq_l f5, 0x0(r5), 0, qr2
/* 8003BCFC 00038CFC  E0 C6 20 00 */	psq_l f6, 0x0(r6), 0, qr2
/* 8003BD00 00038D00  10 A5 00 28 */	ps_sub f5, f5, f0
/* 8003BD04 00038D04  10 C6 00 28 */	ps_sub f6, f6, f0
/* 8003BD08 00038D08  E0 04 20 00 */	psq_l f0, 0x0(r4), 0, qr2
/* 8003BD0C 00038D0C  E1 27 A0 00 */	psq_l f9, 0x0(r7), 1, qr2
/* 8003BD10 00038D10  11 04 01 5C */	ps_madds0 f8, f4, f5, f0
/* 8003BD14 00038D14  10 E2 01 9C */	ps_madds0 f7, f2, f6, f0
/* 8003BD18 00038D18  10 01 01 9C */	ps_madds0 f0, f1, f6, f0
/* 8003BD1C 00038D1C  10 E3 39 5C */	ps_madds0 f7, f3, f5, f7
/* 8003BD20 00038D20  11 49 04 20 */	ps_merge00 f10, f9, f0
/* 8003BD24 00038D24  11 67 44 20 */	ps_merge00 f11, f7, f8
/* 8003BD28 00038D28  F1 43 20 00 */	psq_st f10, 0x0(r3), 0, qr2
/* 8003BD2C 00038D2C  F1 63 20 20 */	psq_st f11, 0x20(r3), 0, qr2
/* 8003BD30 00038D30  11 49 04 60 */	ps_merge01 f10, f9, f0
/* 8003BD34 00038D34  11 67 44 E0 */	ps_merge11 f11, f7, f8
/* 8003BD38 00038D38  F1 43 20 02 */	psq_st f10, 0x2(r3), 0, qr2
/* 8003BD3C 00038D3C  F1 63 20 22 */	psq_st f11, 0x22(r3), 0, qr2
/* 8003BD40 00038D40  E0 04 20 02 */	psq_l f0, 0x2(r4), 0, qr2
/* 8003BD44 00038D44  11 04 01 5E */	ps_madds1 f8, f4, f5, f0
/* 8003BD48 00038D48  10 E2 01 9E */	ps_madds1 f7, f2, f6, f0
/* 8003BD4C 00038D4C  10 01 01 9E */	ps_madds1 f0, f1, f6, f0
/* 8003BD50 00038D50  10 E3 39 5E */	ps_madds1 f7, f3, f5, f7
/* 8003BD54 00038D54  11 49 04 20 */	ps_merge00 f10, f9, f0
/* 8003BD58 00038D58  11 67 44 20 */	ps_merge00 f11, f7, f8
/* 8003BD5C 00038D5C  F1 43 20 04 */	psq_st f10, 0x4(r3), 0, qr2
/* 8003BD60 00038D60  F1 63 20 24 */	psq_st f11, 0x24(r3), 0, qr2
/* 8003BD64 00038D64  11 49 04 60 */	ps_merge01 f10, f9, f0
/* 8003BD68 00038D68  11 67 44 E0 */	ps_merge11 f11, f7, f8
/* 8003BD6C 00038D6C  F1 43 20 06 */	psq_st f10, 0x6(r3), 0, qr2
/* 8003BD70 00038D70  F1 63 20 26 */	psq_st f11, 0x26(r3), 0, qr2
/* 8003BD74 00038D74  4E 80 00 20 */	blr
.endfn fn_8003BCE4
