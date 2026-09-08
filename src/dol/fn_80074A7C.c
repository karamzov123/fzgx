#include "types.h"

extern u32 lbl_801A6D40;

u32 fn_80074A7C(u32 arg0) {
    u32 v0;
    v0 = lbl_801A6D40;
    lbl_801A6D40 = arg0;
    return v0;
}
