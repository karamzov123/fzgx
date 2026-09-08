#include "types.h"

extern u32 lbl_801A6A20;

u32 fn_800206FC(u32 arg0) {
    u32 v0;
    if ((arg0 & 0x1F) != 0) {
    v0 = (arg0 + (32 - (arg0 & 0x1F)));
    lbl_801A6A20 = v0;
    return arg0;
    }
    lbl_801A6A20 = arg0;
}
