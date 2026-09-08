#include "rel/main_rel/globals.h"

// Return the byte at offset 0x8 from the spline object referenced by the global.
u8 fn_1_FA4C0(void) {
    return *(u8 *)(lbl_1_bss_84420 + 0x8);
}
