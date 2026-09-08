#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
} CameraSlot;

// Copies the current camera parameters into the selected camera slot.
void camera_save_slot(u8 index) {
    CameraSlot *dst = (CameraSlot *)&lbl_1_bss_10D8 + index;

    dst->unk_0 = camera_state.unk_0;
    dst->unk_4 = camera_state.unk_4;
    dst->unk_8 = camera_state.unk_8;
    dst->unk_C = camera_state.unk_C;
    dst->unk_10 = camera_state.unk_10;
    dst->unk_14 = camera_state.unk_14;
    dst->unk_18 = camera_state.unk_18;
    dst->unk_1C = camera_state.unk_1C;
    dst->unk_20 = *(u32 *)&camera_state.unk_20;
}
