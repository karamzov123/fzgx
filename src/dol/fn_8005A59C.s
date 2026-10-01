.include "macros.inc"
.file "fn_8005A59C.c"

# 0x8005A59C..0x8005A5A8 | size: 0xC
.text
.balign 4

# .text:0x0 | 0x8005A59C | size: 0xC
.fn fn_8005A59C, global
/* 8005A59C 0005759C  38 00 00 00 */	li r0, 0x0
/* 8005A5A0 000575A0  2C 00 00 28 */	cmpwi r0, 0x28
/* 8005A5A4 000575A4  4E 80 00 20 */	blr
.endfn fn_8005A59C
