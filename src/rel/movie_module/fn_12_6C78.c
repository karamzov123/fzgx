#include "types.h"

extern u32 lbl_12_bss_4DB0;

s32 fn_12_6C78(u32 arg0) {
    u32 v0;
    u32 v1;
    lbl_12_bss_4DB0 = arg0;
    v0 = arg0;
    if (arg0 == 0) {
    v0 = -1;
    return v0;
    }
    v1 = *(u32 *)((u8 *)v0 + 0);
    v0 = (v1 - 1);
    v0 = ((s32)(~(v0 | (1 - v1))) >> 31);
    return v0;
}
