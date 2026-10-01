.include "macros.inc"
.file "fn_8003BD78.c"

# 0x8003BD78..0x8003BE1C | size: 0xA4
.text
.balign 4

# .text:0x0 | 0x8003BD78 | size: 0xA4
.fn fn_8003BD78, global
/* 8003BD78 00038D78  C0 02 83 64 */	lfs f0, lbl_801A71A4@sda21(r0)
/* 8003BD7C 00038D7C  C0 22 83 68 */	lfs f1, lbl_801A71A8@sda21(r0)
/* 8003BD80 00038D80  C0 42 83 6C */	lfs f2, lbl_801A71AC@sda21(r0)
/* 8003BD84 00038D84  C0 62 83 70 */	lfs f3, lbl_801A71B0@sda21(r0)
/* 8003BD88 00038D88  C0 82 83 74 */	lfs f4, lbl_801A71B4@sda21(r0)
/* 8003BD8C 00038D8C  E0 C5 20 00 */	psq_l f6, 0x0(r5), 0, qr2
/* 8003BD90 00038D90  E0 E6 20 00 */	psq_l f7, 0x0(r6), 0, qr2
/* 8003BD94 00038D94  10 C6 00 28 */	ps_sub f6, f6, f0
/* 8003BD98 00038D98  10 E7 00 28 */	ps_sub f7, f7, f0
/* 8003BD9C 00038D9C  E0 A4 20 00 */	psq_l f5, 0x0(r4), 0, qr2
/* 8003BDA0 00038DA0  E1 47 A0 00 */	psq_l f10, 0x0(r7), 1, qr2
/* 8003BDA4 00038DA4  11 26 29 3A */	ps_madd f9, f6, f4, f5
/* 8003BDA8 00038DA8  11 07 28 BA */	ps_madd f8, f7, f2, f5
/* 8003BDAC 00038DAC  10 A7 28 7A */	ps_madd f5, f7, f1, f5
/* 8003BDB0 00038DB0  11 06 40 FA */	ps_madd f8, f6, f3, f8
/* 8003BDB4 00038DB4  10 CA 2C 20 */	ps_merge00 f6, f10, f5
/* 8003BDB8 00038DB8  10 E8 4C 20 */	ps_merge00 f7, f8, f9
/* 8003BDBC 00038DBC  F0 C3 20 00 */	psq_st f6, 0x0(r3), 0, qr2
/* 8003BDC0 00038DC0  F0 E3 20 20 */	psq_st f7, 0x20(r3), 0, qr2
/* 8003BDC4 00038DC4  10 CA 2C 60 */	ps_merge01 f6, f10, f5
/* 8003BDC8 00038DC8  10 E8 4C E0 */	ps_merge11 f7, f8, f9
/* 8003BDCC 00038DCC  F0 C3 20 02 */	psq_st f6, 0x2(r3), 0, qr2
/* 8003BDD0 00038DD0  F0 E3 20 22 */	psq_st f7, 0x22(r3), 0, qr2
/* 8003BDD4 00038DD4  E0 C5 20 02 */	psq_l f6, 0x2(r5), 0, qr2
/* 8003BDD8 00038DD8  E0 E6 20 02 */	psq_l f7, 0x2(r6), 0, qr2
/* 8003BDDC 00038DDC  10 C6 00 28 */	ps_sub f6, f6, f0
/* 8003BDE0 00038DE0  10 E7 00 28 */	ps_sub f7, f7, f0
/* 8003BDE4 00038DE4  E0 A4 20 02 */	psq_l f5, 0x2(r4), 0, qr2
/* 8003BDE8 00038DE8  11 26 29 3A */	ps_madd f9, f6, f4, f5
/* 8003BDEC 00038DEC  11 07 28 BA */	ps_madd f8, f7, f2, f5
/* 8003BDF0 00038DF0  10 A7 28 7A */	ps_madd f5, f7, f1, f5
/* 8003BDF4 00038DF4  11 06 40 FA */	ps_madd f8, f6, f3, f8
/* 8003BDF8 00038DF8  10 CA 2C 20 */	ps_merge00 f6, f10, f5
/* 8003BDFC 00038DFC  10 E8 4C 20 */	ps_merge00 f7, f8, f9
/* 8003BE00 00038E00  F0 C3 20 04 */	psq_st f6, 0x4(r3), 0, qr2
/* 8003BE04 00038E04  F0 E3 20 24 */	psq_st f7, 0x24(r3), 0, qr2
/* 8003BE08 00038E08  10 CA 2C 60 */	ps_merge01 f6, f10, f5
/* 8003BE0C 00038E0C  10 E8 4C E0 */	ps_merge11 f7, f8, f9
/* 8003BE10 00038E10  F0 C3 20 06 */	psq_st f6, 0x6(r3), 0, qr2
/* 8003BE14 00038E14  F0 E3 20 26 */	psq_st f7, 0x26(r3), 0, qr2
/* 8003BE18 00038E18  4E 80 00 20 */	blr
.endfn fn_8003BD78
