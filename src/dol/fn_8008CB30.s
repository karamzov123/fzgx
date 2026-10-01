.include "macros.inc"
.file "fn_8008CB30.c"

# 0x8008CB30..0x8008CB38 | size: 0x8
.text
.balign 4

# .text:0x0 | 0x8008CB30 | size: 0x8
.fn fn_8008CB30, global
/* 8008CB30 00089B30  0F E0 00 00 */	twui r0, 0x0
/* 8008CB34 00089B34  4E 80 00 20 */	blr
.endfn fn_8008CB30
