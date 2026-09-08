#include "types.h"

extern u32 fn_800474E4(u32);
extern u32 lbl_80090DA0[];

u32 fn_8004BE98(u32 arg0, u32 arg1) {
    u32 v0;
    u32 t0;
    v0 = arg0;
    if (arg0 == 0) {
    v0 = (u32)&lbl_80090DA0;
    t0 = fn_800474E4((u32)v0);
    v0 = t0;
    } else {
    *(u32 *)((u8 *)v0 + 56) = arg1;
    }
    return v0;
}
