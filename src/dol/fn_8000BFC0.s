.include "macros.inc"
.file "fn_8000BFC0.c"

# 0x8000BFC0..0x8000BFC8 | size: 0x8
.text
.balign 4

# .text:0x0 | 0x8000BFC0 | size: 0x8
.fn fn_8000BFC0, global
/* 8000BFC0 00008FC0  7C 23 0B 78 */	mr r3, r1
/* 8000BFC4 00008FC4  4E 80 00 20 */	blr
.endfn fn_8000BFC0
