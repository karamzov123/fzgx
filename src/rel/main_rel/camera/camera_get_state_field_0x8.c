#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns the address of the camera state field at offset 0x8.
u32* camera_get_state_field_0x8(void) {
    return &camera_state.unk_8;
}
