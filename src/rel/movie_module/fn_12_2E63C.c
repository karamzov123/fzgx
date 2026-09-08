#include "types.h"

struct fn_12_2E63C_Arg0 {
    u8 pad_0[0x48];
    u32 unk_48;
};
struct fn_12_2E63C_Arg1 {
    u32 unk_0;
};
struct fn_12_2E63C_Arg2 {
    u32 unk_0;
};
struct fn_12_2E63C_lbl_12_bss_7C64 {
    u8 pad_0[0x1B8];
    u32 unk_1B8;
};

extern struct fn_12_2E63C_lbl_12_bss_7C64 lbl_12_bss_7C64;

s32 fn_12_2E63C(struct fn_12_2E63C_Arg0 *arg0, struct fn_12_2E63C_Arg1 *arg1, struct fn_12_2E63C_Arg2 *arg2, u32 arg3) {
    u32 v0;
    s32 v1;
    u32 v2;
    v0 = arg0->unk_48;
    v1 = arg3;
    if ((s32)v0 != 4 && (s32)v0 != -4 && (s32)v0 != 6 && (s32)v0 != -6) {
    v1 = -1;
    arg1->unk_0 = v1;
    v1 = 0;
    arg2->unk_0 = 1;
    } else {
    v1 = 1;
    }
    v2 = (u32)arg0;
    if (v1 == 0) {
    v2 = 0;
    return v2;
    }
    arg1->unk_0 = *(u32 *)((u8 *)v2 + 3876);
    arg2->unk_0 = lbl_12_bss_7C64.unk_1B8;
    return 0;
}
