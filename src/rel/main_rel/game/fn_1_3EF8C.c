#include "types.h"

extern u8 lbl_1_bss_3C12[10];
extern u8 lbl_1_data_5B7C;

void fn_1_3EF8C(u8 value) {
    if (lbl_1_bss_3C12[0] != 0) {
        return;
    }
    lbl_1_data_5B7C = value;
}
