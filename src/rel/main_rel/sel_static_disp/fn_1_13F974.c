#include "types.h"

extern u32 lbl_801A6410;
extern char lbl_1_data_419E0[18];
extern u32 fn_1_45D0(u32 arg0, int arg1, void* arg2, int arg3);
extern u32 lbl_1_bss_8E404[8];
extern u32 fn_80008BEC(u32 arg0, int arg1, int arg2);

u32 fn_1_13F974(u32 arg0) {
    u32* config = &lbl_801A6410;

    lbl_1_bss_8E404[arg0 & 0xFF] =
        fn_1_45D0(config[0], 0x440, lbl_1_data_419E0, 0x1528);
    return fn_80008BEC(lbl_1_bss_8E404[arg0 & 0xFF], 0, 0x440);
}

