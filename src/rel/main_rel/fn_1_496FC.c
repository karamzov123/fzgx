#include "types.h"

extern u32 lbl_1_bss_4B9CC[23];

void fn_1_496FC(f32 value1, f32 value2) {
    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x4) = value1;
    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x0) = value1;
    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x8) = value2;
}
