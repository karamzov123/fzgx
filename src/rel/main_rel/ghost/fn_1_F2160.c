#include "types.h"

extern void fn_1_12EF80(s16 arg, s16 *out_a, s16 *out_b);
extern u8 lbl_1_bss_7F0C0[];

void fn_1_F2160(s32 arg) {
    s16 a;
    s16 b;
    s32 index;

    fn_1_12EF80((s16)arg, &a, &b);
    index = (a - 1) * 6 + b;
    lbl_1_bss_7F0C0[0x4938 + index] |= 4;
}
