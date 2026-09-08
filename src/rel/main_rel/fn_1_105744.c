#include "types.h"

extern u32 lbl_1_bss_854B8[1666];

// fn_1_105744: Initialize the first data structure in lbl_1_bss_854B8
void fn_1_105744(void) {
    s32 neg_one = -1;
    s32 zero = 0;
    u32 *p = lbl_1_bss_854B8;
    *(s16 *)((char *)p + 0xc) = neg_one;
    p[0] = zero;
    p[1] = zero;
    p[2] = zero;
}
