#include "types.h"

extern u32 lbl_1_bss_84420[2];

// fn_1_FA4C0: Load byte from offset 0x8 of pointer at lbl_1_bss_84420[0].
u8 fn_1_FA4C0(void) {
    u32 ptr = lbl_1_bss_84420[0];
    return *(u8 *)(ptr + 0x8);
}
