#include "rel/main_rel/globals.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

extern void fn_1_862D4(u8 index, Vec3 *out);
extern void fn_1_8658C(u8 index, Vec3 *out);

// Computes the displacement between two camera positions.
void fn_1_AD94(u32 index, Vec3 *out) {
    u8 idx = (u8)index;
    Vec3 first;
    Vec3 second;

    fn_1_862D4(idx, &first);
    fn_1_8658C(idx, &second);
    out->x = first.x - second.x;
    out->y = first.y - second.y;
    out->z = first.z - second.z;
}
