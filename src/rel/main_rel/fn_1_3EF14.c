#include "types.h"

extern u8 lbl_1_bss_3C12[10];
extern s16 lbl_1_bss_960;
extern u8 lbl_1_bss_3C30[5308];

extern void fn_80008BA8(void *arg1, u8 *arg2, u32 arg3);
extern void fn_80008BEC(void *arg1, u32 arg2, u32 arg3);

void fn_1_3EF14(void *arg1) {
    if (lbl_1_bss_3C12[0] != 0 ||
        ((u16)(lbl_1_bss_960 - 13) <= 1) ||
        lbl_1_bss_960 == 10 ||
        lbl_1_bss_960 == 16) {
        fn_80008BA8(arg1, lbl_1_bss_3C30, 0x14b8);
    } else {
        fn_80008BEC(arg1, 0, 0x14b8);
    }
}
