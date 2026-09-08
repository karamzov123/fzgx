#include "rel/main_rel/globals.h"

u32 fn_1_660C(void) {
    // Return the camera object's top-bit flag, or zero when no object is active.
    if (lbl_1_bss_F68 != 0) {
        return lbl_1_bss_F68->unk_0 & (1u << 31);
    }

    return 0;
}
