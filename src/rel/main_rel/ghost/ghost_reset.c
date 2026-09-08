#include "types.h"

extern u8 lbl_1_bss_7C8CE[70];
extern u32 lbl_1_data_3E52C;
extern void fn_1_F1D70(void);

void ghost_reset(void) {
    lbl_1_bss_7C8CE[0] = 0;
    lbl_1_data_3E52C = -1;
    fn_1_F1D70();
}
