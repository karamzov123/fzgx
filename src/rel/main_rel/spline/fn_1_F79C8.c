#include "rel/main_rel/globals.h"

extern u8 lbl_1_bss_718E0[23016];
extern u16 lbl_1_data_414[36];
extern void fn_1_F8918(u8 *, Obj_1_bss_7F0C0 *, u8 *);

// Initialize the spline workspace and publish its active data region.
void fn_1_F79C8(void) {
    fn_1_F8918(&lbl_1_bss_718E0[0x1c0], &lbl_1_bss_7F0C0, lbl_1_bss_718E0);
    lbl_1_bss_84420 = (u32)&lbl_1_bss_7F0C0.pad_4918[0x244];
    lbl_1_data_414[0] = 0xffff;
}
