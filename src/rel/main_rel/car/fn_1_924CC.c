#include "types.h"

extern s32 lbl_1_bss_6E984;
extern u32 lbl_1_rodata_3FA0[28];
extern void fn_80007AB4(void *arg0);

void fn_1_924CC(void) {
    u32 value;

    if (lbl_1_bss_6E984 != 0) {
        value = lbl_1_rodata_3FA0[0];
        fn_80007AB4(&value);
    }
}
