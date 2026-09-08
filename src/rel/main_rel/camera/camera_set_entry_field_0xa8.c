#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

void camera_set_entry_field_0xa8(u8 index, s16 value) {
    // Store the selected camera entry's parameter.
    game_camera_entries[index].unk_A8 = value;
}
