#include "types.h"

extern u32 fn_800454E4(u32);

u32 fn_80041578(u32 arg0) {
    u32 v0;
    u32 t0;
    v0 = arg0;
    if (arg0 == 0) {
    v0 = 0;
    } else {
    v0 = *(u32 *)((u8 *)v0 + 4);
    t0 = fn_800454E4(v0);
    v0 = t0;
    }
    return v0;
}
