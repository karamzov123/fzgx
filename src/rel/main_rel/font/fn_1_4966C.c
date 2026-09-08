#include "types.h"

extern u32 lbl_1_bss_4B9CC[23];

void fn_1_4966C(f32 value1, f32 value2) {
    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x3c) = value1;
    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x40) = value2;
}
