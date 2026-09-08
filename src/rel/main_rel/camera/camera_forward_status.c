#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

extern void fn_1_6D2C(u32);

// Forward the camera state's status to the next update stage.
void camera_forward_status(void) {
    Obj_1_bss_F68_Target *state = lbl_1_bss_F68;
    u32 value;

    if (state == 0) {
        value = 0;
    } else if ((state->unk_0 & ((u32)1 << 31)) != 0) {
        value = 0;
    } else {
        value = state->unk_4A;
    }

    fn_1_6D2C(value);
}
