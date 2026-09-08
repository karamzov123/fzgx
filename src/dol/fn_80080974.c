#include "types.h"

extern u32 memcpy(u32, u32, u32);

struct fn_80080974_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

s32 fn_80080974(struct fn_80080974_Arg0 *arg0, u32 arg1, u32 arg2) {
    u32 v0;
    u32 v1;
    u32 v2;
    v0 = arg0->unk_8;
    v1 = *(u32 *)((u8 *)(u32)arg0 + 4);
    v2 = (v1 - v0);
    if ((v0 + arg2) <= v1) {
    v2 = arg2;
    }
    memcpy((*(u32 *)((u8 *)(u32)arg0 + 0) + v0), arg1, v2);
    *(u32 *)((u8 *)(u32)arg0 + 8) = (*(u32 *)((u8 *)(u32)arg0 + 8) + v2);
    return 1;
}
