#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Return the shared camera object used by the camera system.
LiveCamera *live_camera_get(void) {
    return live_camera;
}
