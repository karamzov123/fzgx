#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

s16 fn_1_6AE4(void) {
    Obj_1_bss_F68_Target *state = lbl_1_bss_F68;
    u32 value;

    // Return the camera result only when the state and mode are ready.
    if ((s8)state->unk_48 == 6) {
        if (state == 0) {
            value = 0;
        } else if ((state->unk_0 & ((s32)1 << 31)) != 0) {
            value = 0;
        } else {
            value = state->unk_4A;
        }

        if (value == 3) {
            return lbl_1_bss_F70->unk_6;
        }
    }

    return -1;
}
