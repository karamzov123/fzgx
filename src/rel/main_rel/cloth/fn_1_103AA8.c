#include "types.h"

extern f32 lbl_1_rodata_78E8[19];
extern void fn_1_103B00(void);
extern f32 lbl_1_bss_85290;
extern u32 lbl_1_data_2A7E0[62];

void fn_1_103AA8(void) {
    lbl_1_bss_85290 = lbl_1_rodata_78E8[0];
    lbl_1_data_2A7E0[13] = (u32)fn_1_103B00;
}
