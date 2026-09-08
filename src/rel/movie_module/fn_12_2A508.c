#include "types.h"

extern u32 fn_12_24990(u32);
extern u32 fn_12_24A88(u32, u32);
extern u32 fn_12_2A85C(u32);

u32 fn_12_2A508(u32 arg0) {
    u32 v0;
    u32 t0, t1, t2;
    t0 = fn_12_24990(arg0);
    v0 = t0;
    if ((s32)t0 != 0) {
    v0 = 0;
    t1 = fn_12_24A88(v0, (0xFF000000 + 387));
    v0 = t1;
    v0 = 0;
    } else {
    v0 = arg0;
    t2 = fn_12_2A85C(v0);
    v0 = t2;
    }
    return v0;
}
