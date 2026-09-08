#include "types.h"

extern u8 lbl_1_rodata_FD0[];
extern u8 lbl_1_bss_4BA30[];

// fn_1_4D0A0: initialize bss structure with float values from rodata.
void fn_1_4D0A0(void) {
    f32 f2 = *(f32 *)(lbl_1_rodata_FD0 + 0x16ac);
    *(f32 *)(lbl_1_bss_4BA30 + 0x0) = 1;
    *(f32 *)(lbl_1_bss_4BA30 + 0x4) = f2;
    *(f32 *)(lbl_1_bss_4BA30 + 0x8) = *(f32 *)(lbl_1_rodata_FD0 + 0xf0);
    *(f32 *)(lbl_1_bss_4BA30 + 0xc) = *(f32 *)(lbl_1_rodata_FD0 + 0x16b0);
    *(f32 *)(lbl_1_bss_4BA30 + 0x10) = *(f32 *)(lbl_1_rodata_FD0 + 0xf0);
}
