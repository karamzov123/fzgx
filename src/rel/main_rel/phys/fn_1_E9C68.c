#include "types.h"

extern u32 lbl_1_bss_7B188[2];
extern s16 lbl_1_bss_960;
extern u32 fn_1_3F8C0(void);
extern u8 lbl_1_bss_7C85C[113];
extern u32 lbl_1_data_3E52C;
extern u8 lbl_1_data_3E434[68];
extern u32 fn_80008E84(u32 value);

void fn_1_E9C68(void) {
    u32 value;

    if ((s32)lbl_1_bss_7B188[0] == 0) {
        return;
    }

    if (lbl_1_bss_960 == 9) {
        value = fn_80008E84(fn_1_3F8C0());
        *(u32*)lbl_1_bss_7C85C = value;
    }

    if ((s32)lbl_1_data_3E52C >= 0) {
        ((void (**)(void))lbl_1_data_3E434)[lbl_1_data_3E52C]();
    }

    if (lbl_1_bss_960 == 9) {
        fn_80008E84(*(u32*)lbl_1_bss_7C85C);
    }
}
