.include "macros.inc"
.file "__shr2u.c"

# 0x80079D28..0x80079D4C | size: 0x24
.text
.balign 4

# .text:0x0 | 0x80079D28 | size: 0x24
.fn __shr2u, global
/* 80079D28 00076D28  21 05 00 20 */	subfic r8, r5, 0x20
/* 80079D2C 00076D2C  31 25 FF E0 */	subic r9, r5, 0x20
/* 80079D30 00076D30  7C 84 2C 30 */	srw r4, r4, r5
/* 80079D34 00076D34  7C 6A 40 30 */	slw r10, r3, r8
/* 80079D38 00076D38  7C 84 53 78 */	or r4, r4, r10
/* 80079D3C 00076D3C  7C 6A 4C 30 */	srw r10, r3, r9
/* 80079D40 00076D40  7C 84 53 78 */	or r4, r4, r10
/* 80079D44 00076D44  7C 63 2C 30 */	srw r3, r3, r5
/* 80079D48 00076D48  4E 80 00 20 */	blr
.endfn __shr2u
