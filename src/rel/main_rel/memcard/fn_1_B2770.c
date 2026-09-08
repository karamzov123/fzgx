#include "types.h"

// fn_1_B2770: main_rel .text:0x000B2770 size 0xC
// Store byte 0x3 at offset 3 of the pointer argument

void fn_1_B2770(int unused, void *ptr) {
    *(u8 *)((u8 *)ptr + 3) = 0x3;
}
