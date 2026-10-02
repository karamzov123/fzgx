#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

extern const f32 lbl_801A73D4;
extern const f32 lbl_801A73D8;
extern const f64 lbl_801A73E8;
extern f64 fn_800883E8(f64);
extern f64 fn_80088538(f64);

#pragma fp_contract off
void fn_8006EB4C(Vec4 *arg0, const Vec4 *arg1, const Vec4 *arg2, f32 arg3) {
    f32 bx;
    f32 by;
    f32 bz;
    f32 bw;
    f32 dot;
    f32 w0;
    f32 w1;
    f32 ang;
    f32 s;

    bx = arg2->x;
    dot = arg1->w * (bw = arg2->w) + (arg1->z * (bz = arg2->z) + (arg1->x * bx + arg1->y * (by = arg2->y)));
    if (dot < lbl_801A73D4) {
        dot = -dot;
        bx = -bx;
        by = -by;
        bz = -bz;
        bw = -bw;
    }
    if (lbl_801A73D8 - dot > lbl_801A73E8) {
        ang = (f32)fn_80088538(dot);
        s = (f32)fn_800883E8(ang);
        w0 = lbl_801A73D8 - arg3;
        w0 = fn_800883E8(w0 * ang) / s;
        w1 = fn_800883E8(arg3 * ang) / s;
    } else {
        w0 = lbl_801A73D8 - arg3;
        w1 = arg3;
    }
    arg0->x = w0 * arg1->x + w1 * bx;
    arg0->y = w0 * arg1->y + w1 * by;
    arg0->z = w0 * arg1->z + w1 * bz;
    arg0->w = w0 * arg1->w + w1 * bw;
}
