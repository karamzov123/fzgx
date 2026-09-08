#include "rel/main_rel/globals.h"

u32 fn_1_FA070(void) {
    u32 ptr = lbl_1_bss_84420;
    // Test the object's low flag bit.
    return (*(u8 *)(ptr + 2)) & 1;
}
