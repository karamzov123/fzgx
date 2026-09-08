#include "types.h"

extern u32 fn_8007A440(u32, u32);
extern u32 fn_8007AA7C(void);

u32 fn_8007A294(u32 arg0, u32 arg1) {
    u32 v0;
    u32 t0, t1;
    v0 = arg0;
    if (arg1 == 0) {
    v0 = 0;
    } else {
    if (arg1 > (u32)-49) {
    v0 = 0;
    } else {
    if (arg1 <= 68) {
    t0 = fn_8007A440(v0, arg1);
    v0 = t0;
    } else {
    t1 = fn_8007AA7C();
    v0 = t1;
    }
    }
    }
    return v0;
}
