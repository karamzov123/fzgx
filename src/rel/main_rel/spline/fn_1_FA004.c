#include "rel/main_rel/globals.h"

// Reads the second status byte and returns its bit-mask value.
u32 fn_1_FA004(void) {
    u32 ptr = lbl_1_bss_84420;
    return (*(u8 *)(ptr + 0x1)) & 2;
}
