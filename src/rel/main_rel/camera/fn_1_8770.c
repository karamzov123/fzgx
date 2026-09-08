#include "types.h"

extern u32 lbl_1_bss_F70;
extern u8 lbl_1_data_3318[180];
extern char lbl_1_data_360C[35];

extern void fn_8000C49C(u8 *arg0, ...);

typedef struct CameraState {
    u8 pad_00[0xAC];
    s32 active;
    s32 value_b0;
    u8 pad_B4[0x18];
    f32 value_cc;
    f32 value_d0;
    f32 value_d4;
} CameraState;

void fn_1_8770(s32 value, const f32 *delta) {
    CameraState *state;

    if (lbl_1_bss_F70 == 0) {
        fn_8000C49C(lbl_1_data_3318, 0x89a, lbl_1_data_360C);
    }

    state = (CameraState *)lbl_1_bss_F70;
    state->active = 1;
    state->value_cc += delta[0];
    state->value_d0 += delta[1];
    state->value_d4 += delta[2];

    if (value < 0 || state->value_b0 < 0) {
        state->value_b0 = -1;
    } else if (state->value_b0 < value) {
        state->value_b0 = value;
    }
}
