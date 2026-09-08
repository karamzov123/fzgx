#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Return the current camera target object.
GameCameraEntry *game_camera_get(void) {
    return game_camera_entries;
}
