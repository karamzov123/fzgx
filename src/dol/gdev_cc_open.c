#include "types.h"

extern u32 lbl_801A6E10;

s32 gdev_cc_open(u32 arg0) {
    u32 v0;
    v0 = arg0;
    if ((s32)lbl_801A6E10 != 0) {
    v0 = -10005;
    return v0;
    }
    v0 = 0;
    lbl_801A6E10 = 1;
    return v0;
}
