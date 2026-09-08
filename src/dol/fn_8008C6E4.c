#include "types.h"

extern u32 fn_8008C730(u32, u32, void *, u32, u32);

s32 fn_8008C6E4(u32 arg0, u32 arg1) {
    u32 v0;
    u32 loc_8[2];
    /* frame */
    u32 t0;
    loc_8[0] = 4;
    t0 = fn_8008C730(arg0, arg1, loc_8, 0, 1);
    v0 = t0;
    if ((s32)t0 == 0) {
    if (loc_8[0] != 4) {
    v0 = 1792;
    }
    }
    return v0;
}
