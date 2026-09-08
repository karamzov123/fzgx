#include "types.h"

extern u32 lbl_1_bss_84420[2];

u32 fn_1_FA004(void) {
    u32 ptr = lbl_1_bss_84420[0];
    return (*(u8 *)(ptr + 0x1)) & 2;
}
