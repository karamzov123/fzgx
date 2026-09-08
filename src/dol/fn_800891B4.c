#include "types.h"

extern u32 fn_8008AF40(u32);
extern u32 fn_8008AF48(u32);
extern u32 lbl_801A36E8[];

u32 fn_800891B4(u32 arg0) {
    u32 v0;
    s32 v1;
    u32 t0, t1;
    v0 = arg0;
    if ((s32)arg0 != -1) {
    if ((s32)v0 >= 0) {
    if ((s32)v0 < 3) {
    v1 = (v0 * 2192);
    v0 = (u32)((u8 *)&lbl_801A36E8 + v1);
    t0 = fn_8008AF48((u32)v0);
    v0 = t0;
    v0 = (u32)((u8 *)&lbl_801A36E8 + v1);
    *(u32 *)((u8 *)((u8 *)&lbl_801A36E8 + v1) + 4) = 0;
    t1 = fn_8008AF40((u32)v0);
    v0 = t1;
    }
    }
    }
    return v0;
}
