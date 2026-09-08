#include "types.h"

// fn_1_A96EC: main_rel .text:0x000A96EC size 0x10
// Zero the first element of lbl_1_data_3599C

extern u32 lbl_1_data_3599C[15];

void fn_1_A96EC(void) {
    lbl_1_data_3599C[0] = 0;
}
