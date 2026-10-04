#include "dolphin/types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

struct FnObj {
    u8 pad_1cc[0x1cc];
    s32 count;
    u8 pad_674[0x674 - 0x1d0];
    Vec3 first;
    u8 pad_6f4[0x6f4 - 0x680];
    Vec3 second;
};

extern struct FnObj *lbl_801A66CC;

static inline f32 vec_dist(const volatile f32 *a, const f32 *b) /* volatile: locks the load order of the three components */
{
    f32 dx = a[0];
    f32 dy;
    f32 dz;
    dx -= b[0];
    dy = a[1];
    dy -= b[1];
    dz = a[2];
    dz -= b[2];
    dx = dx * dx;
    dx = dx + dy * dy;
    dx = dx + dz * dz;
    return dx;
}

#pragma opt_common_subs off
s32 fn_1_17A9C(const f32 *arg0)
{
    s32 n;
    struct FnObj *obj = lbl_801A66CC;
    u8 *cur;
    s32 best;
    f32 bd, d;
    s32 i;

    n = obj->count;
    if (n <= 1) {
        best = 0;
    } else {
        bd = vec_dist(arg0, (const f32 *)((u8 *)obj + 0x674));
        best = 1;
        (void) best;  /* fzgx: keeps the web at its definition */
        cur = (u8 *)obj + 0x2b0;
        for (i = 2; i < n; i++) {
            d = vec_dist(arg0, (const f32 *)(cur + 0x444));
            if (bd > d) {
                best = i;
                bd = d;
            }
            cur += 0x80;
        }
    }
    return best;
}
#pragma opt_common_subs reset
