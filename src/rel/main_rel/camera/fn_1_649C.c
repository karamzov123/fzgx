#include "types.h"

typedef struct {
    u8 pad[0x48];
    u8 value;
} CameraState;

extern CameraState *lbl_1_bss_F68;

s32 fn_1_649C(void) {
    CameraState *state = lbl_1_bss_F68;

    if (state == 0) {
        return -1;
    }

    return state->value;
}
