#include "types.h"

extern u32 lbl_12_bss_1BAAC[1131];

s32 fn_12_38A0C(s32 arg) {
    lbl_12_bss_1BAAC[25] = arg;
    return arg & ~(-(s32)(arg == 0));
}
