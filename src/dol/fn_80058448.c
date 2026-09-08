#include "types.h"

extern u32 fn_800576DC(void);
extern u32 fn_80057728(u32);
extern u32 memset(u32, u32, u32);

u32 fn_80058448(u32 arg0, u32 arg1, u32 arg2) {
    u32 v0;
    u32 v1;
    u32 v2;
    u32 t0, t1, t2;
    t0 = fn_80057728(arg0);
    v0 = t0;
    v1 = arg1;
    v2 = arg2;
    if (arg0 != 0) {
    v0 = arg0;
    v1 = 0;
    v2 = 64;
    t1 = memset(v0, v1, v2);
    v0 = t1;
    *(u32 *)((u8 *)arg0 + 4) = 0;
    }
    t2 = fn_800576DC();
    v0 = t2;
    return v0;
}
