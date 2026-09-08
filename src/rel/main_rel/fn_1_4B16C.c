#include "types.h"

extern f32 fn_1_4B1D4(s32 mode, s32 value);

s32 fn_1_4B16C(s32 value) {
    return (s32)fn_1_4B1D4(2, value);
}
