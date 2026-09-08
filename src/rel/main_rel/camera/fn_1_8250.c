#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

s32 fn_1_8250(const u8 *lhs_index, const u8 *rhs_index) {
    f32 *camera_values = &lbl_1_bss_6F524.unk_0;
    f32 lhs_value = camera_values[*lhs_index];
    f32 rhs_value = camera_values[*rhs_index];

    // Orders two camera indices by their associated values.
    if (lhs_value < rhs_value) {
        return -1;
    }
    if (lhs_value == rhs_value) {
        return 0;
    }
    return 1;
}
