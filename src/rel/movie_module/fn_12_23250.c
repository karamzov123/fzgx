#include "types.h"

extern u32 fn_12_2D73C(u32, u32);

u32 fn_12_23250(u32 arg0) {
    u32 t0;
    t0 = fn_12_2D73C(arg0, 49);
    return ((u32)((-t0) | t0) >> 31);
}
