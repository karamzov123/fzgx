#include "types.h"


struct fn_12_6B8_Arg0 {
    u8 pad_0[4];
    s32 unk_4;
    s32 unk_8;
    s32 unk_C;
    u8 pad_10[4];
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    u8 pad_20[4];
    s32 unk_24;
    s32 unk_28;
    s32 unk_2C;
};

void fn_12_6B8(struct fn_12_6B8_Arg0 *arg0, u32 arg1) {
    s32 v0;
    s32 v2;
    s32 t8;
    s32 t4;
    s32 tc;
    s32 t18;
    s32 t1c;
    s32 t14;
    s32 t28;
    s32 t2c;
    s32 t24;
    t8 = arg0->unk_8;
    t4 = arg0->unk_4;
    tc = arg0->unk_C;
    v0 = ((s32)(((u32)arg1 >> 31) + arg1) >> 1);
{
    s32 v1;
    v1 = (v0 << 1);
    arg0->unk_4 = t4 + v1 * t8;
    arg0->unk_C = tc - v1;
    v2 = ((s32)(((v0 >> 30) & 0x1) + v1) >> 1);
}
    t18 = arg0->unk_18;
    t1c = arg0->unk_1C;
    t14 = arg0->unk_14;
    arg0->unk_14 = t14 + v2 * t18;
    arg0->unk_1C = t1c - v2;
    t28 = arg0->unk_28;
    t2c = arg0->unk_2C;
    t24 = arg0->unk_24;
    arg0->unk_24 = t24 + v2 * t28;
    arg0->unk_2C = t2c - v2;
}
