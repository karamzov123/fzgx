#include "types.h"

extern u32 lbl_1_bss_4B9CC[23];
extern u32 lbl_1_data_1AEA8[616];

void fn_1_4955C(f32 value1, f32 value2) {
    s16 index = *(s16 *)((u8 *)lbl_1_bss_4B9CC + 0xc);
    f32 scale = *(f32 *)((u8 *)lbl_1_data_1AEA8 + index * 0x38 + 0x34);

    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x1c) = value1 * scale;
    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x20) = value2 * scale;
}
