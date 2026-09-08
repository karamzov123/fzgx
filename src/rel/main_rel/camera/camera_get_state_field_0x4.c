#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns the address of the camera state field at offset 4.
u32* camera_get_state_field_0x4(void) {
    return &camera_state.unk_4;
}
