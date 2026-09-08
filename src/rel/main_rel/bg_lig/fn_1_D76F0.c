#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

extern u32 lbl_1_data_2A7E0[62];

void fn_1_D76F0(const Vec3 *src, s16 index) {
    f32 *dst = (f32 *)lbl_1_data_2A7E0[15];
    dst[index * 3] = src->x;
    dst += index * 3;
    dst[1] = src->y;
    dst[2] = src->z;
}

