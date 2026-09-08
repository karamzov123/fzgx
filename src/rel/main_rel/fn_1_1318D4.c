#include "types.h"

extern u8 lbl_1_bss_8E380;
extern u8 fn_1_B7C00(void);
extern s32 fn_1_B7CD4(void);
extern s32 fn_1_B7C5C(void);
extern char lbl_1_data_40EF8[17];
extern u32 lbl_1_bss_8E384[17];
extern u8 lbl_1_data_40EA4[84];
extern u32 lbl_801A6410;
extern void OSReport(const char* format, ...);
extern void fn_1_46B4(u32 arg0, u32 arg1, u8* arg2, u32 arg3);

s32 fn_1_1318D4(void) {
    s32 result;

    if (lbl_1_bss_8E380 == 0) {
        return 0;
    }

    if (fn_1_B7C00() != 0) {
        return 1;
    }

    if (fn_1_B7CD4() == 0) {
        result = fn_1_B7C5C();
        OSReport(lbl_1_data_40EF8, result);
    }

    if (lbl_1_bss_8E384[0] != 0) {
        fn_1_46B4(lbl_801A6410, lbl_1_bss_8E384[0], lbl_1_data_40EA4, 0x22b);
        lbl_1_bss_8E384[0] = 0;
    }

    lbl_1_bss_8E380 = 0;
    return 1;
}
