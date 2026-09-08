#include "types.h"

struct fn_80077E0C_Arg0 {
    u8 pad_0[0x20];
    u32 unk_20;
};
struct fn_80077E0C_lbl_801A6D38_T {
    u8 pad_0[0x18];
    u32 unk_18;
};

extern struct fn_80077E0C_lbl_801A6D38_T *lbl_801A6D38;
extern u32 fn_800756B0(u32, u32);
extern u32 fn_80075908(u32, u32);
extern u32 lbl_801A6D90;
extern u32 lbl_801A6D9C;

void fn_80077E0C(struct fn_80077E0C_Arg0 *arg0) {
    u32 v1;
    u32 v2;
    u32 v3;
    u32 v0;
    u32 v4;
    v0 = ((u32)arg0 + 64);
    v1 = arg0->unk_20;
    v2 = lbl_801A6D38->unk_18;
    v3 = ((u32)arg0 + v1);
    v4 = v2;
    if ((s32)v2 != 0) {
    v4 = (v4 ^ 3);
    }
    lbl_801A6D9C = v4;
    if (lbl_801A6D90 == 0) {
    v4 = v3;
    fn_800756B0((u32)arg0, v4);
    }
    v4 = v0;
    fn_80075908(v3, v4);
}
