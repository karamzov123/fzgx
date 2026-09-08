#include "types.h"


struct fn_12_68C_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
    u8 pad_10[0xC];
    u32 unk_1C;
    u8 pad_20[0xC];
    u32 unk_2C;
};

void fn_12_68C(struct fn_12_68C_Arg0 *arg0, u32 arg1) {
    s32 v0;
    u32 v1;
    s32 v2;
    v0 = ((s32)(((u32)arg1 >> 31) + arg1) >> 1);
    v1 = (v0 << 1);
    arg0->unk_C = v1;
    v2 = ((s32)(((v0 >> 30) & 0x1) + v1) >> 1);
    arg0->unk_1C = v2;
    arg0->unk_2C = v2;
}
