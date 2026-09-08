#include "types.h"

extern u32 fn_80007654(u32);

u32 fn_800079C4(u32 arg0) {
    u32 t0;
    t0 = fn_80007654(arg0);
    return ((((arg0 >> 12) & 0xFFFF) ^ (t0 & 0x7FFFF)) << 6);
}
