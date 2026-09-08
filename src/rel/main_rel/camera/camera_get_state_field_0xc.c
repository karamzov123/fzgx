#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns a pointer to the camera object's field at offset 0xC.
u32* camera_get_state_field_0xc(void) {
    return &camera_state.unk_C;
}
