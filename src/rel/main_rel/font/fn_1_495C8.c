#include "types.h"

extern u32 lbl_1_bss_4B9CC[23];

void fn_1_495C8(u8 value) {
    *(u8 *)((u8 *)lbl_1_bss_4B9CC + 0x36) = value;
}
