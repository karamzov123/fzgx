.include "macros.inc"
.file "__shl2i.c"

# 0x80079D04..0x80079D28 | size: 0x24
.text
.balign 4

# .text:0x0 | 0x80079D04 | size: 0x24
.fn __shl2i, global
/* 80079D04 00076D04  21 05 00 20 */	subfic r8, r5, 0x20
/* 80079D08 00076D08  31 25 FF E0 */	subic r9, r5, 0x20
/* 80079D0C 00076D0C  7C 63 28 30 */	slw r3, r3, r5
/* 80079D10 00076D10  7C 8A 44 30 */	srw r10, r4, r8
/* 80079D14 00076D14  7C 63 53 78 */	or r3, r3, r10
/* 80079D18 00076D18  7C 8A 48 30 */	slw r10, r4, r9
/* 80079D1C 00076D1C  7C 63 53 78 */	or r3, r3, r10
/* 80079D20 00076D20  7C 84 28 30 */	slw r4, r4, r5
/* 80079D24 00076D24  4E 80 00 20 */	blr
.endfn __shl2i
