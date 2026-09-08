#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

void camera_set_result(s16 value) {
    // Update the camera result only while the camera state is active.
    if (lbl_1_bss_F68 != 0) {
        game_camera_entries->unk_2 = value;
    }
}
