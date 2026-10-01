.include "macros.inc"
.file "fn_8003BB30.c"

# 0x8003BB30..0x8003BCE4 | size: 0x1B4
.text
.balign 4

# .text:0x0 | 0x8003BB30 | size: 0x1B4
.fn fn_8003BB30, global
/* 8003BB30 00038B30  C0 02 83 54 */	lfs f0, lbl_801A7194@sda21(r0)
/* 8003BB34 00038B34  C0 22 83 58 */	lfs f1, lbl_801A7198@sda21(r0)
/* 8003BB38 00038B38  C0 42 83 5C */	lfs f2, lbl_801A719C@sda21(r0)
/* 8003BB3C 00038B3C  C0 62 83 60 */	lfs f3, lbl_801A71A0@sda21(r0)
/* 8003BB40 00038B40  10 81 14 20 */	ps_merge00 f4, f1, f2
/* 8003BB44 00038B44  10 A1 1C 20 */	ps_merge00 f5, f1, f3
/* 8003BB48 00038B48  28 07 00 00 */	cmplwi r7, 0x0
/* 8003BB4C 00038B4C  38 E7 00 01 */	addi r7, r7, 0x1
/* 8003BB50 00038B50  54 E7 F8 7E */	srwi r7, r7, 1
/* 8003BB54 00038B54  4C 81 00 20 */	blelr
/* 8003BB58 00038B58  54 E0 E8 FF */	srwi. r0, r7, 3
/* 8003BB5C 00038B5C  7C 09 03 A6 */	mtctr r0
/* 8003BB60 00038B60  41 82 01 50 */	beq .L_8003BCB0
.L_8003BB64:
/* 8003BB64 00038B64  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BB68 00038B68  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BB6C 00038B6C  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BB70 00038B70  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BB74 00038B74  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BB78 00038B78  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BB7C 00038B7C  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BB80 00038B80  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BB84 00038B84  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BB88 00038B88  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BB8C 00038B8C  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BB90 00038B90  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BB94 00038B94  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BB98 00038B98  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BB9C 00038B9C  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BBA0 00038BA0  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BBA4 00038BA4  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BBA8 00038BA8  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BBAC 00038BAC  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BBB0 00038BB0  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BBB4 00038BB4  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BBB8 00038BB8  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BBBC 00038BBC  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BBC0 00038BC0  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BBC4 00038BC4  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BBC8 00038BC8  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BBCC 00038BCC  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BBD0 00038BD0  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BBD4 00038BD4  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BBD8 00038BD8  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BBDC 00038BDC  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BBE0 00038BE0  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BBE4 00038BE4  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BBE8 00038BE8  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BBEC 00038BEC  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BBF0 00038BF0  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BBF4 00038BF4  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BBF8 00038BF8  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BBFC 00038BFC  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BC00 00038C00  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BC04 00038C04  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BC08 00038C08  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BC0C 00038C0C  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BC10 00038C10  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BC14 00038C14  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BC18 00038C18  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BC1C 00038C1C  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BC20 00038C20  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BC24 00038C24  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BC28 00038C28  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BC2C 00038C2C  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BC30 00038C30  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BC34 00038C34  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BC38 00038C38  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BC3C 00038C3C  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BC40 00038C40  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BC44 00038C44  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BC48 00038C48  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BC4C 00038C4C  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BC50 00038C50  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BC54 00038C54  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BC58 00038C58  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BC5C 00038C5C  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BC60 00038C60  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BC64 00038C64  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BC68 00038C68  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BC6C 00038C6C  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BC70 00038C70  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BC74 00038C74  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BC78 00038C78  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BC7C 00038C7C  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BC80 00038C80  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BC84 00038C84  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BC88 00038C88  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BC8C 00038C8C  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BC90 00038C90  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BC94 00038C94  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BC98 00038C98  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BC9C 00038C9C  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BCA0 00038CA0  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BCA4 00038CA4  42 00 FE C0 */	bdnz .L_8003BB64
/* 8003BCA8 00038CA8  70 E7 00 07 */	andi. r7, r7, 0x7
/* 8003BCAC 00038CAC  4D 82 00 20 */	beqlr
.L_8003BCB0:
/* 8003BCB0 00038CB0  7C E9 03 A6 */	mtctr r7
.L_8003BCB4:
/* 8003BCB4 00038CB4  E4 24 20 02 */	psq_lu f1, 0x2(r4), 0, qr2
/* 8003BCB8 00038CB8  E4 66 70 02 */	psq_lu f3, 0x2(r6), 0, qr7
/* 8003BCBC 00038CBC  E4 45 70 02 */	psq_lu f2, 0x2(r5), 0, qr7
/* 8003BCC0 00038CC0  10 63 18 54 */	ps_sum0 f3, f3, f1, f3
/* 8003BCC4 00038CC4  10 22 10 56 */	ps_sum1 f1, f2, f1, f2
/* 8003BCC8 00038CC8  10 63 1C A0 */	ps_merge10 f3, f3, f3
/* 8003BCCC 00038CCC  10 21 01 3A */	ps_madd f1, f1, f4, f0
/* 8003BCD0 00038CD0  10 63 01 7A */	ps_madd f3, f3, f5, f0
/* 8003BCD4 00038CD4  F4 23 20 02 */	psq_stu f1, 0x2(r3), 0, qr2
/* 8003BCD8 00038CD8  F4 63 20 02 */	psq_stu f3, 0x2(r3), 0, qr2
/* 8003BCDC 00038CDC  42 00 FF D8 */	bdnz .L_8003BCB4
/* 8003BCE0 00038CE0  4E 80 00 20 */	blr
.endfn fn_8003BB30
