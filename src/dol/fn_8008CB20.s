.include "macros.inc"
.file "fn_8008CB20.c"

# 0x8008CB20..0x8008CB28 | size: 0x8
.text
.balign 4

# .text:0x0 | 0x8008CB20 | size: 0x8
.fn fn_8008CB20, global
/* 8008CB20 00089B20  0F E0 00 00 */	twui r0, 0x0
/* 8008CB24 00089B24  4E 80 00 20 */	blr
.endfn fn_8008CB20
