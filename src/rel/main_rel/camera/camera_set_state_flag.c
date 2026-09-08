#include "rel/main_rel/globals.h"

extern u8 lbl_1_bss_F75;

// Store the camera state flag used by subsequent camera updates.
void camera_set_state_flag(u8 value) {
    lbl_1_bss_F75 = value;
}
