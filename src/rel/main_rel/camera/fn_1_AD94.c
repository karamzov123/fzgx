#include "types.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

extern void fn_1_862D4(u8 index, Vec3 *out);
extern void fn_1_8658C(u8 index, Vec3 *out);

void fn_1_AD94(u32 index, Vec3 *out) {
    u8 idx = (u8)index;
    Vec3 *dst = out;
    Vec3 a;
    Vec3 b;

    fn_1_862D4(idx, &a);
    fn_1_8658C(idx, &b);
    dst->x = a.x - b.x;
    dst->y = a.y - b.y;
    dst->z = a.z - b.z;
}
