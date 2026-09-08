#include "types.h"


struct fn_80089144_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
    u32 unk_C;
};

s32 fn_80089144(struct fn_80089144_Arg0 *arg0, u32 arg1) {
    s32 v0;
    v0 = 0;
    if (arg1 > 2176) {
    v0 = 769;
    } else {
    arg0->unk_C = arg1;
    if (arg1 > arg0->unk_8) {
    arg0->unk_8 = arg1;
    }
    }
    return v0;
}
