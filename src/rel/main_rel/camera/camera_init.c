#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

extern void fn_1_A6FE8(void);
extern void fn_1_8D08(GameCameraEntry *value);

// Initialize camera state before passing the shared camera object onward.
void camera_init(void) {
    fn_1_A6FE8();
    fn_1_8D08(game_camera_entries);
}
