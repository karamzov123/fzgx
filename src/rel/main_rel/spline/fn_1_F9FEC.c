#include "rel/main_rel/globals.h"

// Returns the low flag bit stored in the spline state.
u32 fn_1_F9FEC(void) {
    u32 ptr = lbl_1_bss_84420;
    return (*(u8 *)(ptr + 0x1)) & 1;
}
