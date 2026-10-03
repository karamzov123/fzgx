#include "types.h"

struct fn_80076238_arg1 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct fn_80076238_arg2 {
    u8 unk0[8];
    s32 unk8;
    s32 unkC;
};

struct fn_80076238_Mtx {
    f32 unk_0;
    f32 unk_4;
    f32 unk_8;
    f32 unk_C;
    f32 unk_10;
    f32 unk_14;
    f32 unk_18;
    f32 unk_1C;
    f32 unk_20;
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
};

extern struct fn_80076238_Mtx *lbl_801A6D00;
extern u8 lbl_801A3220[];
extern u8 lbl_8015AD1C[];
extern const f32 lbl_801A7480;
extern const f32 lbl_801A7488;
extern const f32 lbl_801A748C;
extern const f32 lbl_801A749C;

extern void lbl_8006DAEC(void);
extern void lbl_8006DB30(void);
extern void lbl_8006E14C(f32);
extern void GXLoadTexMtxImm(void *, u32, u32);
extern void fn_8006F120(void *, void *);

extern void fn_80073C6C(s32);
extern void fn_80072AB0(s32, s32, s32);
extern void fn_800736C0(s32, void *);
extern void fn_800735C8(s32, s32);
extern void fn_800745A4(u32, s32, s32, u32, u32, u32);
extern void fn_80072C24(s32, s32, s32, s32, s32);
extern void fn_80072D64(s32, s32, s32, s32, u8, s32);
extern void fn_80072CC4(s32, s32, s32, s32, s32);
extern void fn_80072E20(s32, s32, s32, s32, u8, s32);

void fn_80076238(struct fn_80076238_arg1 *p0, struct fn_80076238_arg2 *p1, u8 arg2, s32 arg3) {
    u8 buf[4];
    u32 v;

    fn_80073C6C(p0->unk0);
    fn_80072AB0(p0->unk0, 0, 0);

    if (*(s32 *)(lbl_801A3220 + 0x3C) == 0) {
        lbl_8006DAEC();
        lbl_801A6D00->unk_C = lbl_801A7480;
        lbl_801A6D00->unk_1C = lbl_801A7480;
        lbl_801A6D00->unk_2C = lbl_801A7480;
        GXLoadTexMtxImm(lbl_801A6D00, 30, 0);
        lbl_8006DB30();
        *(s32 *)(lbl_801A3220 + 0x3C) = 1;
    }

    if (*(s32 *)(lbl_801A3220 + 0x40) == 0) {
        lbl_8006DAEC();
        fn_8006F120(lbl_8015AD1C, lbl_801A3220 + 0x50);
        lbl_801A6D00->unk_C = lbl_801A748C;
        lbl_801A6D00->unk_10 = lbl_801A6D00->unk_10 * lbl_801A749C;
        lbl_801A6D00->unk_14 = lbl_801A6D00->unk_14 * lbl_801A749C;
        lbl_801A6D00->unk_18 = lbl_801A6D00->unk_18 * lbl_801A749C;
        lbl_801A6D00->unk_1C = lbl_801A748C;
        lbl_801A6D00->unk_20 = lbl_801A7480;
        lbl_801A6D00->unk_24 = lbl_801A7480;
        lbl_801A6D00->unk_28 = lbl_801A7480;
        lbl_801A6D00->unk_2C = lbl_801A7488;
        lbl_8006E14C(lbl_801A748C);
        GXLoadTexMtxImm(lbl_801A6D00, 64, 0);
        lbl_8006DB30();
        *(s32 *)(lbl_801A3220 + 0x40) = 1;
    }

    buf[0] = arg2;
    buf[1] = arg2;
    buf[2] = arg2;
    buf[3] = arg2;

    fn_800734A8(p0->unk0, p0->unk4, p0->unkC, 4);

    v = *(u32 *)buf;
    fn_800736C0(0, &v);

    fn_800735C8(p0->unk0, 0xC);
    fn_800745A4(p0->unk4, 0, 1, 0x1E, 1, 0x40);

    if (arg3) {
        fn_80072C24(p0->unk0, 0xF, 8, p1->unk8, 0xF);
    } else {
        fn_80072C24(p0->unk0, 0xF, 8, 0xE, p1->unk8);
    }

    fn_80072D64(p0->unk0, 0, 0, 0, 1, 0);
    fn_80072CC4(p0->unk0, 7, 7, 7, p1->unkC);
    fn_80072E20(p0->unk0, 0, 0, 0, 1, 0);

    p0->unk0 = p0->unk0 + 1;
    p0->unk4 = p0->unk4 + 1;
}
