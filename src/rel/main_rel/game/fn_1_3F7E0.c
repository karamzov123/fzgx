#include "types.h"

extern u8 lbl_1_bss_3C30[5308];

u8 fn_1_3F7E0(void) {
    u32 value = *(u32*)lbl_1_bss_3C30;
    if (value & 0x00008000) {
        return lbl_1_bss_3C30[9];
    }
    return lbl_1_bss_3C30[10];
}
