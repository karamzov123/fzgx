#include "types.h"

extern u32 lbl_801A6410;
extern u32 lbl_1_bss_6C840;
extern char lbl_1_data_1C68C[9];
extern void fn_1_46B4(u32, u32, void*, u32);

// fn_1_58158: loads global values and calls fn_1_46B4.
void fn_1_58158(void) {
    u32 v1 = lbl_801A6410;
    u32 v2 = lbl_1_bss_6C840;
    fn_1_46B4(v1, v2, &lbl_1_data_1C68C, 0x3a6);
}
