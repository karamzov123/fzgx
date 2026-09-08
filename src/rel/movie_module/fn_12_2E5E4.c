#include "types.h"

struct fn_12_2E5E4_Arg0 {
    u8 pad_0[0x48];
    u32 unk_48;
};
struct fn_12_2E5E4_Arg1 {
    u32 unk_0;
};
struct fn_12_2E5E4_Arg2 {
    u32 unk_0;
};


s32 fn_12_2E5E4(struct fn_12_2E5E4_Arg0 *arg0, struct fn_12_2E5E4_Arg1 *arg1, struct fn_12_2E5E4_Arg2 *arg2) {
    u32 v0;
    s32 v1;
    v0 = arg0->unk_48;
    v1 = (u32)arg0;
    if ((s32)v0 != 4 && (s32)v0 != -4 && (s32)v0 != 6 && (s32)v0 != -6) {
    v1 = -1;
    arg1->unk_0 = v1;
    v1 = 0;
    arg2->unk_0 = 1;
    } else {
    v1 = 1;
    }
    if (v1 == 0) {
    v1 = 0;
    return v1;
    }
    v1 = 0;
    return v1;
}
