#include "rel/main_rel/globals.h"

typedef struct Camera {
    u8 pad_00[0xA4];
    s16 unk_A4;
} Camera;

extern void fn_1_AEB8(Camera *);

// Resets the camera transition state before refreshing the camera.
void camera_reset_transition(Camera *camera) {
    camera->unk_A4 = 0;
    fn_1_AEB8(camera);
}
