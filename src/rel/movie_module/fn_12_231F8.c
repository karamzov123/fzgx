#include "types.h"

extern u32 fn_12_2D73C(u32, u32);

s32 fn_12_231F8(u32 arg0) {
    u32 v0;
    if ((s32)fn_12_2D73C(arg0, 49) != 0 || (s32)fn_12_2D73C(arg0, 56) != 0) {
    v0 = 1;
    } else {
    v0 = 0;
    }
    return v0;
}
