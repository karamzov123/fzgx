#include "types.h"

// fn_1_7899C: main_rel .text:0x0007899C size 0x10
// Zero the first byte of lbl_1_bss_6D14C

extern u8 lbl_1_bss_6D14C[92];

void fn_1_7899C(void) {
    lbl_1_bss_6D14C[0] = 0;
}
