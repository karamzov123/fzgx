#include "types.h"

extern u32 lbl_801A6D0C;

u32 fn_80070DE0(u32 arg0) {
    u32 v0;
    v0 = lbl_801A6D0C;
    lbl_801A6D0C = arg0;
    return v0;
}
