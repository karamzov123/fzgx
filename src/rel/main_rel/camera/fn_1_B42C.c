#include "types.h"

typedef struct Transform {
    u8 pad_08[0x8];
    f32 x;
    u8 pad_0c[0xc];
    f32 y;
    u8 pad_1c[0xc];
    f32 z;
    u8 pad_2c[0x50];
    u32 flags;
} Transform;

typedef struct CameraState {
    u8 pad_d4[0xd4];
    f32 field_d4;
    f32 field_d8;
    f32 field_dc;
} CameraState;

typedef struct CameraObject {
    u8 pad_49c[0x49c];
    Transform *target;
} CameraObject;

extern f32 lbl_1_rodata_2E0[32];
extern Transform *lbl_801A6D00;
extern CameraState *lbl_801A66CC;
extern f32 lbl_8006D6FC(f32 *value, void *target);

f32 fn_1_B42C(CameraObject *camera) {
    Transform *target = camera->target;

    if (target == 0) {
        return lbl_1_rodata_2E0[0];
    }

    if ((target->flags & 0x01800000) == 0) {
        return lbl_1_rodata_2E0[0];
    }

    lbl_801A66CC->field_d4 = -lbl_801A6D00->x;
    lbl_801A66CC->field_d8 = -lbl_801A6D00->y;
    lbl_801A66CC->field_dc = -lbl_801A6D00->z;
    return lbl_8006D6FC(&lbl_801A66CC->field_d4, (u8 *)target + 0x50);
}
