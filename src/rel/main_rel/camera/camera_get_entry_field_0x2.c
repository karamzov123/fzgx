#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns the selected camera entry value for an 8-bit camera index.
s16 camera_get_entry_field_0x2(u32 index) {
    return game_camera_entries[(u8)index].unk_2;
}
