#include "rel/main_rel/camera.h"

extern void fn_8000C49C(u8 *arg0, ...);

// Marks the camera state active, accumulates a position delta, and tracks the highest value.
void fn_1_8770(s32 value, const f32 *delta) {
    Obj_1_bss_F70_Target *state;

    if (lbl_1_bss_F70 == 0) {
        fn_8000C49C(lbl_1_data_3318, 0x89a, lbl_1_data_360C);
    }

    state = lbl_1_bss_F70;
    state->unk_AC = 1;
    state->unk_CC += delta[0];
    state->unk_D0 += delta[1];
    state->unk_D4 += delta[2];

    if (value < 0 || (s32)state->unk_B0 < 0) {
        state->unk_B0 = -1;
    } else if ((s32)state->unk_B0 < value) {
        state->unk_B0 = value;
    }
}
