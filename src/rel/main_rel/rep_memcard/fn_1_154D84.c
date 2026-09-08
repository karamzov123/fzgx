#include "types.h"

extern u32 lbl_1_bss_8ED90;
extern u32 lbl_1_bss_8ED94[3];
extern u32 lbl_801A6410;
extern char lbl_1_data_49AC8[14];

extern void fn_80008BA8(u32 arg0, u32 arg1, u32 arg2);
extern void fn_1_46B4(u32 arg0, u32 arg1, char *arg2, s32 arg3);

u32 fn_1_154D84(u32 arg0) {
    if (lbl_1_bss_8ED90 == 0) {
        return 0;
    }
    if (arg0 == 0) {
        return 0;
    }

    fn_80008BA8(arg0, lbl_1_bss_8ED90, lbl_1_bss_8ED94[0]);
    fn_1_46B4(lbl_801A6410, lbl_1_bss_8ED90, lbl_1_data_49AC8, 0x13f);
    lbl_1_bss_8ED90 = 0;
    lbl_1_bss_8ED94[0] = 0;
    return 1;
}
