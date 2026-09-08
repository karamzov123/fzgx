#include "types.h"

extern struct fn_16_B10_lbl_1_bss_8B614 lbl_1_bss_8B614;

struct fn_16_B10_lbl_1_bss_8B614 {
    u8 pad_0[0x58];
    u16 unk_58;
};

void * fn_16_B10(void) {
    lbl_1_bss_8B614.unk_58 = 2;
    return &lbl_1_bss_8B614;
}
