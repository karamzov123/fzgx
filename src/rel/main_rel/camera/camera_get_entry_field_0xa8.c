#include "rel/main_rel/camera.h"

s16 camera_get_entry_field_0xa8(u32 index) {
    // Return the selected camera entry's stored value.
    return game_camera_entries[(u8)index].unk_A8;
}
