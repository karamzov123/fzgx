#include "rel/main_rel/globals.h"

extern u8 lbl_1_bss_F74;

// Cache the selected camera value for subsequent camera processing.
void camera_set_selected_value(u8 value) {
    lbl_1_bss_F74 = value;
}
