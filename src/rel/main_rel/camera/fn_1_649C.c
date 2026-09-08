#include "rel/main_rel/globals.h"

s32 fn_1_649C(void) {
    // Return the camera state byte, or -1 when no camera state is active.
    if (lbl_1_bss_F68 == 0) {
        return -1;
    }

    return lbl_1_bss_F68->unk_48;
}
