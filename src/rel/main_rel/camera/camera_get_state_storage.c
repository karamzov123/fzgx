#include "rel/main_rel/globals.h"

extern u8 lbl_1_bss_103C[4];

// Returns the camera state storage used by the surrounding camera code.
u8* camera_get_state_storage(void) {
    return lbl_1_bss_103C;
}
