#include "rel/main_rel/camera.h"

typedef struct {
    u8 pad[0xa8];
    s16 value;
    u8 tail[0x152];
} CameraEntry;

void fn_1_713C(u8 index, s16 value) {
    CameraEntry *entries = (CameraEntry *)lbl_1_bss_F6C;

    // Store a camera parameter in the selected camera entry.
    entries[index].value = value;
}
