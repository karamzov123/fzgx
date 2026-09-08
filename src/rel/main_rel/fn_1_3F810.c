#include "types.h"

extern u8 lbl_1_bss_3C30[5308];

void fn_1_3F810(u8 value, u32 input) {
    lbl_1_bss_3C30[0x146C] = value;
    lbl_1_bss_3C30[0x146D] = (u8)((input / 5) >> 1);
}
