#include "types.h"

extern u32 lbl_801A6E08;

s32 ddh_cc_open(u32 arg0) {
    u32 v0;
    v0 = arg0;
    if ((s32)lbl_801A6E08 != 0) {
    v0 = -10005;
    return v0;
    }
    v0 = 0;
    lbl_801A6E08 = 1;
    return v0;
}
