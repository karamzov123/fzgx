#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

extern void fn_8000C49C(u8 *file, int line, u8 *message, ...);
extern void fn_1_8A0C(s16 index);

// Updates the selected camera state with a movement delta and tracks its highest value.
void game_camera_set_shake(s16 index, s16 mode, s32 value, const f32 *delta) {
    GameCameraEntry *camera;

    if (index < 0) {
        return;
    }

    if (game_camera_entries + index == 0) {
        fn_8000C49C(lbl_1_data_3318, 0x8d1, lbl_1_data_3654);
    }

    camera = game_camera_entries + index;
    if (mode != camera->unk_2) {
        fn_1_8A0C(index);
        return;
    }

    camera->unk_10C = 1;
    camera->unk_12C += delta[0];
    camera->unk_130 += delta[1];
    camera->unk_134 += delta[2];
    camera->unk_150 += delta[2];
    camera->unk_154 += delta[1];
    camera->unk_158 += delta[0];

    if (value < 0 || (s32)camera->unk_110 < 0) {
        camera->unk_110 = -1;
    } else if ((s32)camera->unk_110 < value) {
        camera->unk_110 = value;
    }
}
