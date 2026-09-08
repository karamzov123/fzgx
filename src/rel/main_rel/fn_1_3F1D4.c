#include "types.h"

extern u8 lbl_1_bss_3C30[5308];

s32 fn_1_3F1D4(void) {
    if (lbl_1_bss_3C30[5] == 1) {
        return lbl_1_bss_3C30[0x13f6];
    }
    return -1;
}
