#include "types.h"

extern u32 lbl_1_bss_7E9E8[6];

u32 ghost_test_flag(s32 index) {
    if (index < 0x20) {
        return lbl_1_bss_7E9E8[0] & (1 << index);
    }
    if (index < 0x40) {
        return lbl_1_bss_7E9E8[1] & (1 << (index - 0x20));
    }
    if (index < 0x60) {
        return lbl_1_bss_7E9E8[2] & (1 << (index - 0x40));
    }
    return lbl_1_bss_7E9E8[3] & (1 << (index - 0x60));
}
