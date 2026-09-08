#include "types.h"

extern u8 lbl_1_bss_3C30[5308];

// fn_1_3FC8C: Extract bit 6 from the first word of the global buffer.
u32 fn_1_3FC8C(void) {
    u32 value = *(u32*)lbl_1_bss_3C30;
    return value & 0x02000000;
}
