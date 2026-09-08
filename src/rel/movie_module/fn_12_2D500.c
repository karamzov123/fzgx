#include "types.h"

extern u32 fn_12_24990(u32, u32, u32);
extern u32 fn_12_24A88(u32, u32);

s32 fn_12_2D500(u32 arg0, u32 arg1, u32 arg2) {
    u32 v0;
    u32 t0, t1;
    t0 = fn_12_24990(arg0, arg1, arg2);
    v0 = t0;
    if ((s32)t0 != 0) {
    v0 = 0;
    t1 = fn_12_24A88(v0, (0xFF000000 + 279));
    v0 = t1;
    } else {
    v0 = (arg0 + (arg1 * 68));
    v0 = *(u32 *)((u8 *)v0 + 6824);
    if (v0 == 0) {
    *(u32 *)((u8 *)arg2 + 0) = 0;
    } else {
    *(u32 *)((u8 *)arg2 + 0) = *(u32 *)((u8 *)v0 + 0);
    }
    v0 = 0;
    }
    return v0;
}
