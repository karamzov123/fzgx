#include "rel/main_rel/globals.h"

// Reports whether the camera state is available and not marked as disabled.
u32 fn_1_6F3C(void) {
    Obj_1_bss_F68_Target *state = lbl_1_bss_F68;

    if (state == 0) {
        return 0;
    }
    if ((state->unk_0 & ((u32)1 << 31)) != 0) {
        return 0;
    }
    return (s8)state->unk_48 == 0x0B;
}
