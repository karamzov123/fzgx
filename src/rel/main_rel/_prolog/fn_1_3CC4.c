#include "types.h"

extern u8 lbl_1_bss_D78;
extern u8 lbl_1_bss_DA6;

void fn_1_3CC4(void) {
    if (lbl_1_bss_D78 == 0) {
        return;
    }
    if (lbl_1_bss_DA6 == 0) {
        lbl_1_bss_DA6 = 1;
    }
}
