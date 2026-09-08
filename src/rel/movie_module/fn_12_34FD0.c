#include "types.h"

extern u32 fn_8004AEE4(void);

u32 fn_12_34FD0(void) {
    u32 t0;
    t0 = fn_8004AEE4();
    return ((u32)__cntlzw((4 - t0)) >> 5);
}
