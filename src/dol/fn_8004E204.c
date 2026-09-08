#include "types.h"


struct fn_8004E204_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
};

s32 fn_8004E204(struct fn_8004E204_Arg0 *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    u32 v0;
    v0 = (u32)arg0;
    if ((s32)arg0->unk_C == 0) {
    *(u32 *)((u8 *)v0 + 20) = 2;
    *(u32 *)((u8 *)v0 + 24) = arg1;
    *(u32 *)((u8 *)v0 + 28) = arg2;
    *(u32 *)((u8 *)v0 + 32) = arg3;
    *(u32 *)((u8 *)v0 + 36) = arg4;
    v0 = 1;
    return v0;
    }
    v0 = 0;
    return v0;
}
