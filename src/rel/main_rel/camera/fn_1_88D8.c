#include "types.h"

typedef struct CameraData {
    u8 pad_00[2];
    s16 field_02;
    u8 pad_04[0x108];
    s32 field_10c;
    s32 field_110;
    u8 pad_114[0x18];
    f32 field_12c;
    f32 field_130;
    f32 field_134;
    u8 pad_138[0x18];
    f32 field_150;
    f32 field_154;
    f32 field_158;
    u8 pad_15c[0xa0];
} CameraData;

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec;

extern u8 *lbl_1_bss_F6C;
extern u8 lbl_1_data_3318[180];
extern char lbl_1_data_3654[35];

extern void fn_8000C49C(u8 *file, int line, char *message, ...);
extern void fn_1_8A0C(s16 index);

void fn_1_88D8(s16 index, s16 mode, s32 value, Vec *delta) {
    CameraData *camera;

    if (index < 0) {
        return;
    }

    if (lbl_1_bss_F6C + index * 0x1fc == (u8 *)0) {
        fn_8000C49C(lbl_1_data_3318, 0x8d1, lbl_1_data_3654);
    }

    camera = (CameraData *)(lbl_1_bss_F6C + index * 0x1fc);
    if (mode != camera->field_02) {
        fn_1_8A0C(index);
        return;
    }

    camera->field_10c = 1;
    camera->field_12c += delta->x;
    camera->field_130 += delta->y;
    camera->field_134 += delta->z;
    camera->field_150 += delta->z;
    camera->field_154 += delta->y;
    camera->field_158 += delta->x;

    if (value < 0 || camera->field_110 < 0) {
        camera->field_110 = -1;
    } else if (camera->field_110 < value) {
        camera->field_110 = value;
    }
}
