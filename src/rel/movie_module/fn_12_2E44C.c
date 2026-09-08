#include "types.h"


struct fn_12_2E44C_Arg0 {
    u8 pad_0[0x48];
    u32 unk_48;
};
struct fn_12_2E44C_Arg1 {
    u32 unk_0;
};
struct fn_12_2E44C_Arg2 {
    u32 unk_0;
};

s32 fn_12_2E44C(struct fn_12_2E44C_Arg0 *arg0, struct fn_12_2E44C_Arg1 *arg1, struct fn_12_2E44C_Arg2 *arg2) {
    u32 v0;
    u32 v1;
    v0 = arg0->unk_48;
    v1 = (u32)arg0;
    if ((s32)v0 != 4) {
    if ((s32)v0 != -4) {
    if ((s32)v0 != 6) {
    if ((s32)v0 != -6) {
    v1 = -1;
    arg1->unk_0 = v1;
    v1 = 0;
    arg2->unk_0 = 1;
    return v1;
    }
    }
    }
    }
    v1 = 1;
    return v1;
}
