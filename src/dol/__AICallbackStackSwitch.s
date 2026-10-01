.include "macros.inc"
.file "__AICallbackStackSwitch.c"

# 0x8001E5A8..0x8001E600 | size: 0x58
.text
.balign 4

# .text:0x0 | 0x8001E5A8 | size: 0x58
.fn __AICallbackStackSwitch, global
/* 8001E5A8 0001B5A8  7C 08 02 A6 */	mflr r0
/* 8001E5AC 0001B5AC  90 01 00 04 */	stw r0, 0x4(r1)
/* 8001E5B0 0001B5B0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8001E5B4 0001B5B4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8001E5B8 0001B5B8  7C 7F 1B 78 */	mr r31, r3
/* 8001E5BC 0001B5BC  3C A0 80 1A */	lis r5, lbl_801A69AC@ha
/* 8001E5C0 0001B5C0  38 A5 69 AC */	addi r5, r5, lbl_801A69AC@l
/* 8001E5C4 0001B5C4  90 25 00 00 */	stw r1, 0x0(r5)
/* 8001E5C8 0001B5C8  3C A0 80 1A */	lis r5, lbl_801A69A8@ha
/* 8001E5CC 0001B5CC  38 A5 69 A8 */	addi r5, r5, lbl_801A69A8@l
/* 8001E5D0 0001B5D0  80 25 00 00 */	lwz r1, 0x0(r5)
/* 8001E5D4 0001B5D4  38 21 FF F8 */	subi r1, r1, 0x8
/* 8001E5D8 0001B5D8  7F E8 03 A6 */	mtlr r31
/* 8001E5DC 0001B5DC  4E 80 00 21 */	blrl
/* 8001E5E0 0001B5E0  3C A0 80 1A */	lis r5, lbl_801A69AC@ha
/* 8001E5E4 0001B5E4  38 A5 69 AC */	addi r5, r5, lbl_801A69AC@l
/* 8001E5E8 0001B5E8  80 25 00 00 */	lwz r1, 0x0(r5)
/* 8001E5EC 0001B5EC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8001E5F0 0001B5F0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8001E5F4 0001B5F4  38 21 00 18 */	addi r1, r1, 0x18
/* 8001E5F8 0001B5F8  7C 08 03 A6 */	mtlr r0
/* 8001E5FC 0001B5FC  4E 80 00 20 */	blr
.endfn __AICallbackStackSwitch
