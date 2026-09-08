#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns whether the camera state flag is set.
u8 camera_are_flags_enabled(void) {
    return camera_flag_1;
}
