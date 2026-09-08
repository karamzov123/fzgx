#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

void fn_1_6CB4(s16 value) {
    // Update the camera result only while the camera state is active.
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F6C->unk_2 = value;
    }
}
