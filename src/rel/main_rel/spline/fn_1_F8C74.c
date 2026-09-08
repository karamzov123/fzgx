#include "types.h"

extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8C74(s32 value) {
    value *= 2;
    lbl_1_bss_7F0C0[4676] |= (((u32)1 << 31) >> (31 - (value + 1)));
}
