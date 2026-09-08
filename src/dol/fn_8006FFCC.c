#include "types.h"

extern u32 lbl_801A6CB0;

u32 fn_8006FFCC(u32 arg0) {
    u32 v0;
    v0 = lbl_801A6CB0;
    lbl_801A6CB0 = arg0;
    return v0;
}
