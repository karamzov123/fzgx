#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Clears the camera state flags.
void camera_disable_flags(void) {
    camera_flag_1 = 0;
    camera_flag_0 = 0;
}
