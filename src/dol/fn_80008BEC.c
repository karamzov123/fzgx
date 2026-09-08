#include "types.h"

extern u32 fn_800793D4(u32);
extern u32 memset(u32, u32);

void fn_80008BEC(u32 arg0, u32 arg1) {
    u32 v0;
    u32 t0;
    v0 = arg1;
    if ((arg0 & 0x1F) == 0) {
    t0 = fn_800793D4(arg0);
    } else {
    v0 = (v0 & 0xFF);
    memset(t0, v0);
    }
}
