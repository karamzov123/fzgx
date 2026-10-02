#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec;

extern u32 lbl_8006D5A4(void *, void *, void *, f32);
extern void lbl_8006D668(void *);

static inline void cross(Vec *a, Vec *b, Vec *out) {
    f32 ax = a->x;
    f32 ay = a->y;
    f32 az = a->z;
    f32 bx = b->x;
    f32 bz = b->z;
    f32 by = b->y;
    f32 x;
    f32 y;
    f32 z;
    x = ay * bz;
    x -= az * by;
    y = az * bx;
    y -= ax * bz;
    z = ax * by;
    z -= ay * bx;
    out->x = x;
    out->y = y;
    out->z = z;
}

void fn_8006FA24(f32 arg0, Vec *arg1, Vec *arg2, Vec *arg3) {
    lbl_8006D5A4(&arg1[0], &arg2[0], &arg3[0], arg0);
    lbl_8006D5A4(&arg1[1], &arg2[1], &arg3[1], arg0);
    cross(&arg3[0], &arg3[1], &arg3[2]);
    cross(&arg3[2], &arg3[0], &arg3[1]);
    lbl_8006D668(&arg3[0]);
    lbl_8006D668(&arg3[1]);
    lbl_8006D668(&arg3[2]);
}
