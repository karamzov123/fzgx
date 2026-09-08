#include "types.h"

typedef struct {
    u32 x;
    u32 y;
    u32 z;
} Vec3Bits;

extern u32 lbl_1_data_2A7E0[62];

void fn_1_D7724(Vec3Bits *dst, s16 index) {
    u32 *base = (u32 *)lbl_1_data_2A7E0[15];
    Vec3Bits *src = (Vec3Bits *)(base + index * 3);
    *dst = *src;
}
