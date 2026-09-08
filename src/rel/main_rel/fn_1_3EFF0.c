#include "types.h"

extern u32 lbl_1_bss_25E74[7];
extern u8 lbl_1_bss_3C10;
extern u8 lbl_1_bss_3C11;
extern u8 lbl_1_bss_26C58;
extern u32 lbl_1_bss_26C64;

void fn_1_3EFF0(u32 arg1, u8 arg2) {
    lbl_1_bss_25E74[0] = arg1;
    lbl_1_bss_3C10 = arg2;
    lbl_1_bss_3C11 = 1;
    lbl_1_bss_26C58 = 0;
    lbl_1_bss_26C64 = 0;
}
