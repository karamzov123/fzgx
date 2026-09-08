#include "types.h"

struct fn_80041460_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

extern u32 fn_800454B4(u32, u32);
extern u32 fn_800454CC(u32, u32);

s32 fn_80041460(struct fn_80041460_Arg0 *arg0, u32 arg1) {
    u32 v0;
    u32 t0, t1;
    t0 = fn_800454CC(arg0->unk_4, arg1);
    v0 = t0;
    if ((s32)t0 > 0 && ((s8)*(u8 *)((u8 *)(u32)arg0 + 1) == 2 || (s8)*(u8 *)((u8 *)(u32)arg0 + 1) == 3)) {
    v0 = *(u32 *)((u8 *)(u32)arg0 + 4);
    t1 = fn_800454B4(v0, arg1);
    v0 = t1;
    } else {
    v0 = -128;
    }
    return v0;
}
