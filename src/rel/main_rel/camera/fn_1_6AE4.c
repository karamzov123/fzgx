#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

s16 fn_1_6AE4(void) {
    Obj_1_bss_F68_Target *state = lbl_1_bss_F68;
    u32 result;

    // Return the camera output only when the state is active and ready.
    if ((s8)state->unk_48 == 6) {
        if (state == 0) {
            result = 0;
        } else if ((state->unk_0 & ((s32)1 << 31)) != 0) {
            result = 0;
        } else {
            result = state->unk_4A;
        }

        if (result == 3) {
            return lbl_1_bss_F70->unk_6;
        }
    }

    return -1;
}
