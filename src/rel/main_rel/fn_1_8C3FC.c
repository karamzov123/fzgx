#include "types.h"

// fn_1_8C3FC: Load byte from BSS and return

extern u8 lbl_1_bss_6DA7B[5];

u8 fn_1_8C3FC(void) {
    return lbl_1_bss_6DA7B[0];
}
