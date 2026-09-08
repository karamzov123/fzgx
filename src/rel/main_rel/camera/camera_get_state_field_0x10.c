#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns a pointer to the camera object's state field.
u32* camera_get_state_field_0x10(void) {
    return &camera_state.unk_10;
}
