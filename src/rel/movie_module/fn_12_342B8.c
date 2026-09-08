#include "types.h"

extern u32 fn_80056C20(void);

u32 fn_12_342B8(void) {
    u32 t0;
    t0 = fn_80056C20();
    return ((u32)__cntlzw((3 - t0)) >> 5);
}
