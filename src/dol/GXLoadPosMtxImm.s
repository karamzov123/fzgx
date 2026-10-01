.include "macros.inc"
.file "GXLoadPosMtxImm.c"

# 0x80038C5C..0x80038CAC | size: 0x50
.text
.balign 4

# .text:0x0 | 0x80038C5C | size: 0x50
.fn GXLoadPosMtxImm, global
/* 80038C5C 00035C5C  3C A0 CC 01 */	lis r5, 0xcc01
/* 80038C60 00035C60  38 00 00 10 */	li r0, 0x10
/* 80038C64 00035C64  54 84 10 3A */	slwi r4, r4, 2
/* 80038C68 00035C68  98 05 80 00 */	stb r0, -0x8000(r5)
/* 80038C6C 00035C6C  64 80 00 0B */	oris r0, r4, 0xb
/* 80038C70 00035C70  90 05 80 00 */	stw r0, -0x8000(r5)
/* 80038C74 00035C74  38 85 80 00 */	addi r4, r5, -0x8000
/* 80038C78 00035C78  E0 A3 00 00 */	psq_l f5, 0x0(r3), 0, qr0
/* 80038C7C 00035C7C  E0 83 00 08 */	psq_l f4, 0x8(r3), 0, qr0
/* 80038C80 00035C80  E0 63 00 10 */	psq_l f3, 0x10(r3), 0, qr0
/* 80038C84 00035C84  E0 43 00 18 */	psq_l f2, 0x18(r3), 0, qr0
/* 80038C88 00035C88  E0 23 00 20 */	psq_l f1, 0x20(r3), 0, qr0
/* 80038C8C 00035C8C  E0 03 00 28 */	psq_l f0, 0x28(r3), 0, qr0
/* 80038C90 00035C90  F0 A4 00 00 */	psq_st f5, 0x0(r4), 0, qr0
/* 80038C94 00035C94  F0 84 00 00 */	psq_st f4, 0x0(r4), 0, qr0
/* 80038C98 00035C98  F0 64 00 00 */	psq_st f3, 0x0(r4), 0, qr0
/* 80038C9C 00035C9C  F0 44 00 00 */	psq_st f2, 0x0(r4), 0, qr0
/* 80038CA0 00035CA0  F0 24 00 00 */	psq_st f1, 0x0(r4), 0, qr0
/* 80038CA4 00035CA4  F0 04 00 00 */	psq_st f0, 0x0(r4), 0, qr0
/* 80038CA8 00035CA8  4E 80 00 20 */	blr
.endfn GXLoadPosMtxImm
