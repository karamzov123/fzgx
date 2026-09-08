#include "types.h"

extern u8 lbl_1_bss_3C30[5308];

void fn_1_3F844(u16 value) {
    *(u16 *)&lbl_1_bss_3C30[0x146e] = value;
}
