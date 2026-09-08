#include "types.h"

// fn_1_AC6E0: Write struct fields at offsets 0x8, 0xa, and 0x2a (with OR).
void fn_1_AC6E0(void *r3) {
    *(u16 *)((u8 *)r3 + 0x8) = 0x34;
    *(u16 *)((u8 *)r3 + 0xa) = 0xe;
    *(((u8 *)r3) + 0x2a) |= 0x4;
}
