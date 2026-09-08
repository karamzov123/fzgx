#include "types.h"

extern u32 fn_800565FC(u32, ...);
extern u32 lbl_80092188[];
extern u32 lbl_800921AC[];

u32 fn_800567EC(u32 arg0, u32 arg1) {
    u32 v0;
    u32 t0, t1;
    v0 = arg0;
    if (arg0 == 0) {
    v0 = (u32)&lbl_80092188;
    t0 = fn_800565FC((u32)v0);
    v0 = t0;
    } else {
    if ((s32)arg1 < 0 || (s32)arg1 > (s32)*(u32 *)((u8 *)v0 + 24)) {
    v0 = (u32)&lbl_800921AC;
    t1 = fn_800565FC((u32)v0, arg1);
    v0 = t1;
    } else {
    *(u32 *)((u8 *)v0 + 20) = arg1;
    }
    }
    return v0;
}
