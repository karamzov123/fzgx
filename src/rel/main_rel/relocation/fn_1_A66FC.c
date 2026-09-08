#include "types.h"

extern u32 lbl_1_bss_6F5F4;
extern s32 *lbl_1_data_341B8;
extern u32 lbl_1_bss_6F5F0;
extern u8 lbl_801A66B0[];

extern s32 OSGetResetCode(void);
extern u32 fn_8000FE78(void);
extern void fn_8000FEE8(s32);
extern void fn_1_A6870(u32 *);
extern void fn_1_A5F44(void);
extern s32 fn_1_A6480(void);

s32 fn_1_A66FC(s32 value) {
    s32 result;

    lbl_1_bss_6F5F4 = value;
    if (((u32)__cntlzw((u32)((-2147483647 - 1) - OSGetResetCode())) >> 5) != 0) {
        if (fn_8000FE78() != 0) {
            result = 1;
        } else {
            fn_8000FEE8(0);
            *lbl_1_data_341B8 = 0;
            result = 1;
        }
        if (lbl_801A66B0[3] != 0) {
            lbl_1_bss_6F5F0 = 1;
            fn_1_A6870(&lbl_1_bss_6F5F0);
        }
    } else if (*lbl_1_data_341B8 != 0) {
        fn_1_A5F44();
        result = fn_1_A6480();
    } else {
        fn_8000FEE8(0);
        *lbl_1_data_341B8 = 0;
        result = 1;
    }
    return result;
}
