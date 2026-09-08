#include "types.h"

typedef struct {
    u32 unk_0;
    u8 pad_4[0x46];
    u8 unk_4A;
} CameraState;

extern CameraState *lbl_1_bss_F68;
extern s16 fn_1_6B48(s16 value);

s16 fn_1_6A80(void) {
    CameraState *state = lbl_1_bss_F68;
    s16 value;

    if (state == 0) {
        value = 0;
    } else if ((state->unk_0 & ((s32)1 << 31)) != 0) {
        value = 0;
    } else {
        value = state->unk_4A;
    }

    value = fn_1_6B48(value);
    if (value == 0xff) {
        value = -1;
    }

    return value;
}
