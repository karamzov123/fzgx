#include "types.h"

extern u32 lbl_1_bss_4C678[2];

// fn_1_51990: Copy a value to structure field at +0x4c, then set bit 0x01000000 in field at +0x30.
void fn_1_51990(void *obj) {
    *(u32 *)((u8 *)obj + 0x4c) = lbl_1_bss_4C678[0];
    *(u32 *)((u8 *)obj + 0x30) |= 0x01000000;
}
