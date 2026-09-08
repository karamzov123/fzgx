#include "types.h"

extern u32 lbl_801A6D90;

u32 fn_80077BAC(u32 arg0) {
    u32 v0;
    v0 = lbl_801A6D90;
    lbl_801A6D90 = arg0;
    return v0;
}
