#include "types.h"

extern s32 lbl_1_bss_8CA44[10];
extern s32 lbl_1_bss_8CA40;
extern void (*lbl_1_data_40D50[8])(void);

void fn_1_130F98(void) {
    if (lbl_1_bss_8CA44[0] >= 0) {
        s32 value = lbl_1_bss_8CA44[0];

        lbl_1_bss_8CA44[0] = -1;
        lbl_1_bss_8CA40 = value;
    }

    if (lbl_1_bss_8CA40 >= 0) {
        lbl_1_data_40D50[lbl_1_bss_8CA40]();
    }
}
