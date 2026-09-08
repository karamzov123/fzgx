#include "types.h"

extern u32 lbl_12_bss_7C5C;

s32 fn_12_24990(u32 arg0) {
    u32 v0;
    lbl_12_bss_7C5C = arg0;
    v0 = arg0;
    if (arg0 == 0) {
    v0 = -1;
    return v0;
    }
    v0 = (-((__cntlzw(*(u32 *)((u8 *)v0 + 72)) >> 5) & 0x1));
    return v0;
}
