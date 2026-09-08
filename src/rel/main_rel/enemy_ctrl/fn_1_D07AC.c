#include "types.h"

extern u8 lbl_1_bss_7ACA0[48];

void fn_1_D07AC(u32 index, u32 value) {
    ((u32 *)lbl_1_bss_7ACA0)[(index & 0xff) * 3] = value;
}
