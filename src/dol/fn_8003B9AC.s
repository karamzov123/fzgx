.include "macros.inc"
.file "fn_8003B9AC.c"

# 0x8003B9AC..0x8003BB30 | size: 0x184
.text
.balign 4

# .text:0x0 | 0x8003B9AC | size: 0x184
.fn fn_8003B9AC, global
/* 8003B9AC 000389AC  28 07 00 00 */	cmplwi r7, 0x0
/* 8003B9B0 000389B0  C0 02 83 54 */	lfs f0, lbl_801A7194@sda21(r0)
/* 8003B9B4 000389B4  38 E7 00 03 */	addi r7, r7, 0x3
/* 8003B9B8 000389B8  C0 22 83 58 */	lfs f1, lbl_801A7198@sda21(r0)
/* 8003B9BC 000389BC  C0 42 83 5C */	lfs f2, lbl_801A719C@sda21(r0)
/* 8003B9C0 000389C0  54 E7 F0 BE */	srwi r7, r7, 2
/* 8003B9C4 000389C4  C0 62 83 60 */	lfs f3, lbl_801A71A0@sda21(r0)
/* 8003B9C8 000389C8  4C 81 00 20 */	blelr
/* 8003B9CC 000389CC  54 E0 F0 BF */	srwi. r0, r7, 2
/* 8003B9D0 000389D0  7C 09 03 A6 */	mtctr r0
/* 8003B9D4 000389D4  41 82 01 10 */	beq .L_8003BAE4
.L_8003B9D8:
/* 8003B9D8 000389D8  E4 84 20 02 */	psq_lu f4, 0x2(r4), 0, qr2
/* 8003B9DC 000389DC  E4 C5 20 02 */	psq_lu f6, 0x2(r5), 0, qr2
/* 8003B9E0 000389E0  10 84 00 7A */	ps_madd f4, f4, f1, f0
/* 8003B9E4 000389E4  10 C6 00 BA */	ps_madd f6, f6, f2, f0
/* 8003B9E8 000389E8  E4 A4 20 02 */	psq_lu f5, 0x2(r4), 0, qr2
/* 8003B9EC 000389EC  E4 E6 20 02 */	psq_lu f7, 0x2(r6), 0, qr2
/* 8003B9F0 000389F0  10 A5 00 7A */	ps_madd f5, f5, f1, f0
/* 8003B9F4 000389F4  10 E7 00 FA */	ps_madd f7, f7, f3, f0
/* 8003B9F8 000389F8  11 04 34 20 */	ps_merge00 f8, f4, f6
/* 8003B9FC 000389FC  10 84 3C A0 */	ps_merge10 f4, f4, f7
/* 8003BA00 00038A00  F5 03 20 02 */	psq_stu f8, 0x2(r3), 0, qr2
/* 8003BA04 00038A04  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BA08 00038A08  10 85 34 60 */	ps_merge01 f4, f5, f6
/* 8003BA0C 00038A0C  10 A5 3C E0 */	ps_merge11 f5, f5, f7
/* 8003BA10 00038A10  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BA14 00038A14  F4 A3 20 02 */	psq_stu f5, 0x2(r3), 0, qr2
/* 8003BA18 00038A18  E4 84 20 02 */	psq_lu f4, 0x2(r4), 0, qr2
/* 8003BA1C 00038A1C  E4 C5 20 02 */	psq_lu f6, 0x2(r5), 0, qr2
/* 8003BA20 00038A20  10 84 00 7A */	ps_madd f4, f4, f1, f0
/* 8003BA24 00038A24  10 C6 00 BA */	ps_madd f6, f6, f2, f0
/* 8003BA28 00038A28  E4 A4 20 02 */	psq_lu f5, 0x2(r4), 0, qr2
/* 8003BA2C 00038A2C  E4 E6 20 02 */	psq_lu f7, 0x2(r6), 0, qr2
/* 8003BA30 00038A30  10 A5 00 7A */	ps_madd f5, f5, f1, f0
/* 8003BA34 00038A34  10 E7 00 FA */	ps_madd f7, f7, f3, f0
/* 8003BA38 00038A38  11 04 34 20 */	ps_merge00 f8, f4, f6
/* 8003BA3C 00038A3C  10 84 3C A0 */	ps_merge10 f4, f4, f7
/* 8003BA40 00038A40  F5 03 20 02 */	psq_stu f8, 0x2(r3), 0, qr2
/* 8003BA44 00038A44  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BA48 00038A48  10 85 34 60 */	ps_merge01 f4, f5, f6
/* 8003BA4C 00038A4C  10 A5 3C E0 */	ps_merge11 f5, f5, f7
/* 8003BA50 00038A50  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BA54 00038A54  F4 A3 20 02 */	psq_stu f5, 0x2(r3), 0, qr2
/* 8003BA58 00038A58  E4 84 20 02 */	psq_lu f4, 0x2(r4), 0, qr2
/* 8003BA5C 00038A5C  E4 C5 20 02 */	psq_lu f6, 0x2(r5), 0, qr2
/* 8003BA60 00038A60  10 84 00 7A */	ps_madd f4, f4, f1, f0
/* 8003BA64 00038A64  10 C6 00 BA */	ps_madd f6, f6, f2, f0
/* 8003BA68 00038A68  E4 A4 20 02 */	psq_lu f5, 0x2(r4), 0, qr2
/* 8003BA6C 00038A6C  E4 E6 20 02 */	psq_lu f7, 0x2(r6), 0, qr2
/* 8003BA70 00038A70  10 A5 00 7A */	ps_madd f5, f5, f1, f0
/* 8003BA74 00038A74  10 E7 00 FA */	ps_madd f7, f7, f3, f0
/* 8003BA78 00038A78  11 04 34 20 */	ps_merge00 f8, f4, f6
/* 8003BA7C 00038A7C  10 84 3C A0 */	ps_merge10 f4, f4, f7
/* 8003BA80 00038A80  F5 03 20 02 */	psq_stu f8, 0x2(r3), 0, qr2
/* 8003BA84 00038A84  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BA88 00038A88  10 85 34 60 */	ps_merge01 f4, f5, f6
/* 8003BA8C 00038A8C  10 A5 3C E0 */	ps_merge11 f5, f5, f7
/* 8003BA90 00038A90  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BA94 00038A94  F4 A3 20 02 */	psq_stu f5, 0x2(r3), 0, qr2
/* 8003BA98 00038A98  E4 84 20 02 */	psq_lu f4, 0x2(r4), 0, qr2
/* 8003BA9C 00038A9C  E4 C5 20 02 */	psq_lu f6, 0x2(r5), 0, qr2
/* 8003BAA0 00038AA0  10 84 00 7A */	ps_madd f4, f4, f1, f0
/* 8003BAA4 00038AA4  10 C6 00 BA */	ps_madd f6, f6, f2, f0
/* 8003BAA8 00038AA8  E4 A4 20 02 */	psq_lu f5, 0x2(r4), 0, qr2
/* 8003BAAC 00038AAC  E4 E6 20 02 */	psq_lu f7, 0x2(r6), 0, qr2
/* 8003BAB0 00038AB0  10 A5 00 7A */	ps_madd f5, f5, f1, f0
/* 8003BAB4 00038AB4  10 E7 00 FA */	ps_madd f7, f7, f3, f0
/* 8003BAB8 00038AB8  11 04 34 20 */	ps_merge00 f8, f4, f6
/* 8003BABC 00038ABC  10 84 3C A0 */	ps_merge10 f4, f4, f7
/* 8003BAC0 00038AC0  F5 03 20 02 */	psq_stu f8, 0x2(r3), 0, qr2
/* 8003BAC4 00038AC4  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BAC8 00038AC8  10 85 34 60 */	ps_merge01 f4, f5, f6
/* 8003BACC 00038ACC  10 A5 3C E0 */	ps_merge11 f5, f5, f7
/* 8003BAD0 00038AD0  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BAD4 00038AD4  F4 A3 20 02 */	psq_stu f5, 0x2(r3), 0, qr2
/* 8003BAD8 00038AD8  42 00 FF 00 */	bdnz .L_8003B9D8
/* 8003BADC 00038ADC  70 E7 00 03 */	andi. r7, r7, 0x3
/* 8003BAE0 00038AE0  4D 82 00 20 */	beqlr
.L_8003BAE4:
/* 8003BAE4 00038AE4  7C E9 03 A6 */	mtctr r7
.L_8003BAE8:
/* 8003BAE8 00038AE8  E4 84 20 02 */	psq_lu f4, 0x2(r4), 0, qr2
/* 8003BAEC 00038AEC  E4 C5 20 02 */	psq_lu f6, 0x2(r5), 0, qr2
/* 8003BAF0 00038AF0  10 84 00 7A */	ps_madd f4, f4, f1, f0
/* 8003BAF4 00038AF4  10 C6 00 BA */	ps_madd f6, f6, f2, f0
/* 8003BAF8 00038AF8  E4 A4 20 02 */	psq_lu f5, 0x2(r4), 0, qr2
/* 8003BAFC 00038AFC  E4 E6 20 02 */	psq_lu f7, 0x2(r6), 0, qr2
/* 8003BB00 00038B00  10 A5 00 7A */	ps_madd f5, f5, f1, f0
/* 8003BB04 00038B04  10 E7 00 FA */	ps_madd f7, f7, f3, f0
/* 8003BB08 00038B08  11 04 34 20 */	ps_merge00 f8, f4, f6
/* 8003BB0C 00038B0C  10 84 3C A0 */	ps_merge10 f4, f4, f7
/* 8003BB10 00038B10  F5 03 20 02 */	psq_stu f8, 0x2(r3), 0, qr2
/* 8003BB14 00038B14  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BB18 00038B18  10 85 34 60 */	ps_merge01 f4, f5, f6
/* 8003BB1C 00038B1C  10 A5 3C E0 */	ps_merge11 f5, f5, f7
/* 8003BB20 00038B20  F4 83 20 02 */	psq_stu f4, 0x2(r3), 0, qr2
/* 8003BB24 00038B24  F4 A3 20 02 */	psq_stu f5, 0x2(r3), 0, qr2
/* 8003BB28 00038B28  42 00 FF C0 */	bdnz .L_8003BAE8
/* 8003BB2C 00038B2C  4E 80 00 20 */	blr
.endfn fn_8003B9AC
