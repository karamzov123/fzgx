#include "types.h"

extern u32 fn_80046718(u32);
extern u32 fn_80046738(void);
extern u32 memset(u32, u32, u32);

u32 fn_800466D4(u32 arg0) {
    u32 v0;
    u32 t0, t1, t2;
    v0 = arg0;
    if (arg0 != 0) {
    t0 = fn_80046738();
    v0 = t0;
    v0 = arg0;
    t1 = memset(v0, 0, 48);
    v0 = t1;
    t2 = fn_80046718(v0);
    v0 = t2;
    }
    return v0;
}
