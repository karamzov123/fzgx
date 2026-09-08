#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

extern s16 fn_1_6B48(s16 value);

// Return the normalized camera status, treating inactive states as zero.
s16 fn_1_6A80(void) {
    Obj_1_bss_F68_Target *state = lbl_1_bss_F68;
    s16 value;

    if (state == 0) {
        value = 0;
    } else if ((state->unk_0 & ((s32)1 << 31)) != 0) {
        value = 0;
    } else {
        value = state->unk_4A;
    }

    value = fn_1_6B48(value);
    if (value == 0xff) {
        value = -1;
    }

    return value;
}
