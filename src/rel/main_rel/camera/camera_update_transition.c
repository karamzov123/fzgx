#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

typedef struct Camera {
    u8 pad_00[0x78];
    s16 unk_78;
    u8 pad_7A[0x2A];
    s16 unk_A4;
} Camera;

extern void fn_1_AEB8(Camera *);
extern void fn_1_AFC8(Camera *);

// Advances the camera's transition state and updates its active view.
void camera_update_transition(Camera *camera) {
    if (camera->unk_78 == 0) {
        camera->unk_A4++;
        if (camera->unk_A4 >= camera_transition_count) {
            camera->unk_A4 = 0;
        }
        fn_1_AEB8(camera);
    }

    fn_1_AFC8(camera);

    if (camera->unk_78 != 0) {
        camera->unk_78--;
    }
}
