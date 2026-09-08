#include "types.h"

extern u32 fn_80041684(u32);
extern u32 fn_800474E4(u32);
extern u32 lbl_80090CB4[];

s32 fn_8004C0B4(u32 arg0) {
    u32 v0;
    u32 t0, t1;
    v0 = arg0;
    if (arg0 == 0) {
    v0 = (u32)&lbl_80090CB4;
    t0 = fn_800474E4((u32)v0);
    v0 = t0;
    v0 = -1;
    } else {
    if ((s8)*(u8 *)((u8 *)v0 + 1) >= 2) {
    v0 = *(u32 *)((u8 *)v0 + 4);
    t1 = fn_80041684(v0);
    v0 = t1;
    } else {
    v0 = 0;
    }
    }
    return v0;
}
