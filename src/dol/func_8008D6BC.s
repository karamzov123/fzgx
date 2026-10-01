.include "macros.inc"
.file "func_8008D6BC.c"

# 0x80078D48..0x80078D60 | size: 0x18
.text
.balign 4

# .text:0x0 | 0x80078D48 | size: 0x18
.fn func_8008D6BC, global
/* 80078D48 00075D48  3C A0 CC 00 */	lis r5, 0xcc00
/* 80078D4C 00075D4C  38 80 00 61 */	li r4, 0x61
/* 80078D50 00075D50  60 A5 80 00 */	ori r5, r5, 0x8000
/* 80078D54 00075D54  98 85 00 00 */	stb r4, 0x0(r5)
/* 80078D58 00075D58  90 65 00 00 */	stw r3, 0x0(r5)
/* 80078D5C 00075D5C  4E 80 00 20 */	blr
.endfn func_8008D6BC
