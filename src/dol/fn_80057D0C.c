#include "types.h"

struct fn_80057D0C_lbl_8018AE18 {
    u32 unk_0;
};

extern struct fn_80057D0C_lbl_8018AE18 lbl_8018AE18[];
extern u32 lbl_8018AE1C[];
extern u32 memset(u32, u32, u32);

u32 fn_80057D0C(u32 arg0, u32 arg1) {
    u32 v0;
    u32 v1;
    u32 t0;
    v0 = arg0;
    v1 = arg1;
    if ((s32)lbl_8018AE18[0].unk_0 == 0) {
    v1 = 0;
    v0 = (u32)&lbl_8018AE1C;
    t0 = memset((u32)v0, v1, 1152);
    v0 = t0;
    }
    v1 = (u32)&lbl_8018AE18;
    v0 = *(u32 *)((u8 *)v1 + 0);
    *(u32 *)((u8 *)v1 + 0) = (v0 + 1);
    return v0;
}
