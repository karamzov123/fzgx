#include "types.h"
#include "rel/main_rel/globals.h"

typedef struct Transform {
    u8 pad_08[0x8];
    f32 unk_08;
    u8 pad_0c[0xc];
    f32 unk_18;
    u8 pad_1c[0xc];
    f32 unk_28;
    u8 pad_2c[0x24];
    u8 unk_50[0x2c];
    u32 unk_7C;
} Transform;

typedef struct CameraState {
    u8 pad_d4[0xd4];
    f32 unk_D4;
    f32 unk_D8;
    f32 unk_DC;
} CameraState;

typedef struct CameraObject {
    u8 pad_49c[0x49c];
    Transform *unk_49C;
} CameraObject;

extern f32 lbl_1_rodata_2E0[32];
extern Transform *lbl_801A6D00;
extern CameraState *lbl_801A66CC;
extern f32 lbl_8006D6FC(f32 *value, void *target);

// Updates the camera orientation from the active target transform.
f32 fn_1_B42C(CameraObject *camera) {
    Transform *target = camera->unk_49C;

    if (target == 0) {
        return lbl_1_rodata_2E0[0];
    }
    if ((target->unk_7C & 0x01800000) == 0) {
        return lbl_1_rodata_2E0[0];
    }

    lbl_801A66CC->unk_D4 = -lbl_801A6D00->unk_08;
    lbl_801A66CC->unk_D8 = -lbl_801A6D00->unk_18;
    lbl_801A66CC->unk_DC = -lbl_801A6D00->unk_28;
    return lbl_8006D6FC(&lbl_801A66CC->unk_D4, &target->unk_50);
}
