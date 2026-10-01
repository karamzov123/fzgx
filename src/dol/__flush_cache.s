.include "macros.inc"
.file "__flush_cache.c"

# 0x80003424..0x80003458 | size: 0x34
.section .init, "ax"
.balign 4

# .init:0x0 | 0x80003424 | size: 0x34
.fn __flush_cache, global
/* 80003424 00000424  3C A0 FF FF */	lis r5, 0xffff
/* 80003428 00000428  60 A5 FF F1 */	ori r5, r5, 0xfff1
/* 8000342C 0000042C  7C A5 18 38 */	and r5, r5, r3
/* 80003430 00000430  7C 65 18 50 */	subf r3, r5, r3
/* 80003434 00000434  7C 84 1A 14 */	add r4, r4, r3
.L_80003438:
/* 80003438 00000438  7C 00 28 6C */	dcbst r0, r5
/* 8000343C 0000043C  7C 00 04 AC */	sync
/* 80003440 00000440  7C 00 2F AC */	icbi r0, r5
/* 80003444 00000444  30 A5 00 08 */	addic r5, r5, 0x8
/* 80003448 00000448  34 84 FF F8 */	subic. r4, r4, 0x8
/* 8000344C 0000044C  40 80 FF EC */	bge .L_80003438
/* 80003450 00000450  4C 00 01 2C */	isync
/* 80003454 00000454  4E 80 00 20 */	blr
.endfn __flush_cache
