#include "types.h"

extern u32 lbl_1_bss_7B188[2];
extern s32 lbl_1_data_3E52C;

int fn_1_EB200(void) {
    if ((s32)lbl_1_bss_7B188[0] == 0) {
        return 0;
    }

    if (lbl_1_data_3E52C == 14 ||
        lbl_1_data_3E52C == 16 ||
        lbl_1_data_3E52C == 15) {
        return 1;
    }

    return 0;
}
