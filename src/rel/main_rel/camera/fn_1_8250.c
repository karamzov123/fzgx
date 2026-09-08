#include "types.h"

extern f32 lbl_1_bss_6F524[30];

s32 fn_1_8250(const u8 *a, const u8 *b) {
    f32 lhs = lbl_1_bss_6F524[*a];
    f32 rhs = lbl_1_bss_6F524[*b];

    if (lhs < rhs) {
        return -1;
    }
    if (lhs == rhs) {
        return 0;
    }
    return 1;
}
