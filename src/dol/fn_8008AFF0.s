.include "macros.inc"
.file "fn_8008AFF0.c"

# 0x8008AFF0..0x8008B028 | size: 0x38
.text
.balign 4

# .text:0x0 | 0x8008AFF0 | size: 0x38
.fn fn_8008AFF0, global
/* 8008AFF0 00087FF0  3C A0 FF FF */	lis r5, 0xffff
/* 8008AFF4 00087FF4  60 A5 FF F1 */	ori r5, r5, 0xfff1
/* 8008AFF8 00087FF8  7C A5 18 38 */	and r5, r5, r3
/* 8008AFFC 00087FFC  7C 65 18 50 */	subf r3, r5, r3
/* 8008B000 00088000  7C 84 1A 14 */	add r4, r4, r3
.L_8008B004:
/* 8008B004 00088004  7C 00 28 6C */	dcbst r0, r5
/* 8008B008 00088008  7C 00 28 AC */	dcbf r0, r5
/* 8008B00C 0008800C  7C 00 04 AC */	sync
/* 8008B010 00088010  7C 00 2F AC */	icbi r0, r5
/* 8008B014 00088014  30 A5 00 08 */	addic r5, r5, 0x8
/* 8008B018 00088018  34 84 FF F8 */	subic. r4, r4, 0x8
/* 8008B01C 0008801C  40 80 FF E8 */	bge .L_8008B004
/* 8008B020 00088020  4C 00 01 2C */	isync
/* 8008B024 00088024  4E 80 00 20 */	blr
.endfn fn_8008AFF0
