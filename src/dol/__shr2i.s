.include "macros.inc"
.file "__shr2i.c"

# 0x80079D4C..0x80079D74 | size: 0x28
.text
.balign 4

# .text:0x0 | 0x80079D4C | size: 0x28
.fn __shr2i, global
/* 80079D4C 00076D4C  21 05 00 20 */	subfic r8, r5, 0x20
/* 80079D50 00076D50  35 25 FF E0 */	subic. r9, r5, 0x20
/* 80079D54 00076D54  7C 84 2C 30 */	srw r4, r4, r5
/* 80079D58 00076D58  7C 6A 40 30 */	slw r10, r3, r8
/* 80079D5C 00076D5C  7C 84 53 78 */	or r4, r4, r10
/* 80079D60 00076D60  7C 6A 4E 30 */	sraw r10, r3, r9
/* 80079D64 00076D64  40 81 00 08 */	ble .L_80079D6C
/* 80079D68 00076D68  7C 84 53 78 */	or r4, r4, r10
.L_80079D6C:
/* 80079D6C 00076D6C  7C 63 2E 30 */	sraw r3, r3, r5
/* 80079D70 00076D70  4E 80 00 20 */	blr
.endfn __shr2i
