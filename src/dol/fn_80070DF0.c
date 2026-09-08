#include "types.h"

extern u32 lbl_801A6D08;

u32 fn_80070DF0(u32 arg0) {
    u32 v0;
    v0 = lbl_801A6D08;
    lbl_801A6D08 = arg0;
    return v0;
}
