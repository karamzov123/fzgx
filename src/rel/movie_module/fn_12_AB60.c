#include "types.h"

extern u32 fn_12_A660(u32, u32);
extern u32 fn_12_CAB4(u32, u32);

struct fn_12_AB60_Copy80 { u32 a[20]; };

s32 fn_12_AB60(u32 arg0, u32 arg1) {
    u32 v0;
    u32 t0, t1;
    t0 = fn_12_CAB4(arg0, arg1);
    v0 = t0;
    if ((s32)t0 != 0) {
    v0 = 0;
    t1 = fn_12_A660(v0, (0xFF030000 + 524));
    v0 = t1;
    } else {
    *(struct fn_12_AB60_Copy80 *)arg1 = *(struct fn_12_AB60_Copy80 *)((u8 *)arg0 + 464);
    v0 = 0;
    }
    return v0;
}
