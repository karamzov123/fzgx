#include "types.h"

extern u16 lbl_1_bss_7F0C0[5336];

// fn_1_F9E10: Clear bit from u16 at offset 0x6a based on shift amount in lower byte.
void fn_1_F9E10(u32 param) {
    u16 *ptr = (u16*)((u8*)lbl_1_bss_7F0C0 + 0x6a);
    u32 shift_bits = param & 0x000000FF;
    s32 mask = 0x8000 >> shift_bits;
    u32 value = *ptr;
    *ptr = value & ~mask;
}
