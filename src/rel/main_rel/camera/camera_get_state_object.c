#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns the camera state object's base address.
CameraState* camera_get_state_object(void) {
    return &camera_state;
}
