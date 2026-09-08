#include "types.h"

struct fn_80046BE0_Arg2 {
    u16 unk_0;
};


s32 fn_80046BE0(u32 arg0, u32 arg1, struct fn_80046BE0_Arg2 *arg2) {
    u32 v0;
    u16 v1;
    v0 = arg0;
    if ((s32)arg1 < 18) {
    v0 = -1;
    return v0;
    }
    if (*(u16 *)((u8 *)v0 + 0) != 32768) {
    v0 = -2;
    return v0;
    }
    if (*(s16 *)((u8 *)v0 + 2) < 14) {
    v0 = -1;
    return v0;
    }
    v1 = *(u16 *)((u8 *)v0 + 16);
    v0 = 0;
    arg2->unk_0 = v1;
    return v0;
}
