#include "types.h"

extern u32 lbl_1_bss_4B9CC[23];

// fn_1_49738: store value to BSS at offset 0x54
void fn_1_49738(u32 value) {
    *(u32 *)((u8 *)lbl_1_bss_4B9CC + 0x54) = value;
}
