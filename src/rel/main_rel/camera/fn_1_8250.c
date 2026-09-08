#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

s32 fn_1_8250(const u8 *a, const u8 *b) {
    f32 *values = &lbl_1_bss_6F524.unk_0;
    f32 lhs = values[*a];
    f32 rhs = values[*b];

    // Compare the values associated with two camera indices.
    if (lhs < rhs) {
        return -1;
    }
    if (lhs == rhs) {
        return 0;
    }
    return 1;
}
