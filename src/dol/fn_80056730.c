#include "types.h"

extern u32 fn_800565FC(u32, ...);
extern u32 lbl_80092188[];

u32 fn_80056730(u32 arg0, u32 arg1) {
    u32 v0;
    u32 t0;
    v0 = arg0;
    if (arg0 == 0) {
    v0 = (u32)&lbl_80092188;
    t0 = fn_800565FC((u32)v0);
    v0 = t0;
    } else {
    *(u8 *)((u8 *)v0 + 3) = arg1;
    }
    return v0;
}
