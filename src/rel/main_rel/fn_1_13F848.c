#include "types.h"

extern u32 lbl_801A6410;
extern char lbl_1_data_419E0[18];
extern u32 fn_1_45D0(u32 arg0, int arg1, void* arg2, int arg3);
extern u32 lbl_1_bss_8E3E4[8];
extern void fn_1_FC414(u32 arg0, int arg1);

void fn_1_13F848(u32 arg0) {
    u32 index = arg0 & 0xFF;

    lbl_1_bss_8E3E4[index] =
        fn_1_45D0(lbl_801A6410, 0x10440, lbl_1_data_419E0, 0x14F9);
    fn_1_FC414(lbl_1_bss_8E3E4[index], 1);
}
