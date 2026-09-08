#include "types.h"


struct fn_8004B974_Arg2 {
    u32 unk_0;
};

s32 fn_8004B974(u32 arg0, u32 arg1, struct fn_8004B974_Arg2 *arg2) {
    u32 v0;
    v0 = arg0;
    if ((s32)arg1 < 2) {
    v0 = 0;
    return v0;
    }
    if (*(u16 *)((u8 *)v0 + 0) != 32769) {
    v0 = 0;
    return v0;
    }
    arg2->unk_0 = arg1;
    v0 = 1;
    return v0;
}
