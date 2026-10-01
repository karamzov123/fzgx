.include "macros.inc"
.file "GXLoadNrmMtxImm.c"

# 0x80038CAC..0x80038CFC | size: 0x50
.text
.balign 4

# .text:0x0 | 0x80038CAC | size: 0x50
.fn GXLoadNrmMtxImm, global
/* 80038CAC 00035CAC  1C A4 00 03 */	mulli r5, r4, 0x3
/* 80038CB0 00035CB0  3C 80 CC 01 */	lis r4, 0xcc01
/* 80038CB4 00035CB4  38 00 00 10 */	li r0, 0x10
/* 80038CB8 00035CB8  38 A5 04 00 */	addi r5, r5, 0x400
/* 80038CBC 00035CBC  98 04 80 00 */	stb r0, -0x8000(r4)
/* 80038CC0 00035CC0  64 A0 00 08 */	oris r0, r5, 0x8
/* 80038CC4 00035CC4  94 04 80 00 */	stwu r0, -0x8000(r4)
/* 80038CC8 00035CC8  E0 A3 00 00 */	psq_l f5, 0x0(r3), 0, qr0
/* 80038CCC 00035CCC  C0 83 00 08 */	lfs f4, 0x8(r3)
/* 80038CD0 00035CD0  E0 63 00 10 */	psq_l f3, 0x10(r3), 0, qr0
/* 80038CD4 00035CD4  C0 43 00 18 */	lfs f2, 0x18(r3)
/* 80038CD8 00035CD8  E0 23 00 20 */	psq_l f1, 0x20(r3), 0, qr0
/* 80038CDC 00035CDC  C0 03 00 28 */	lfs f0, 0x28(r3)
/* 80038CE0 00035CE0  F0 A4 00 00 */	psq_st f5, 0x0(r4), 0, qr0
/* 80038CE4 00035CE4  D0 84 00 00 */	stfs f4, 0x0(r4)
/* 80038CE8 00035CE8  F0 64 00 00 */	psq_st f3, 0x0(r4), 0, qr0
/* 80038CEC 00035CEC  D0 44 00 00 */	stfs f2, 0x0(r4)
/* 80038CF0 00035CF0  F0 24 00 00 */	psq_st f1, 0x0(r4), 0, qr0
/* 80038CF4 00035CF4  D0 04 00 00 */	stfs f0, 0x0(r4)
/* 80038CF8 00035CF8  4E 80 00 20 */	blr
.endfn GXLoadNrmMtxImm
