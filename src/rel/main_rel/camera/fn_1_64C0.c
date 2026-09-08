#include "rel/main_rel/globals.h"

u32 fn_1_6514(u32);

// Return the camera status unless the camera is absent or marked inactive.
u32 fn_1_64C0(void) {
    Obj_1_bss_F68_Target *state = lbl_1_bss_F68;
    u32 value;

    if (state == 0) {
        value = 0;
    } else if ((state->unk_0 & ((u32)1 << 31)) != 0) {
        value = 0;
    } else {
        value = state->unk_4A;
    }

    return fn_1_6514(value);
}
