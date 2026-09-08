#include "types.h"

extern u8 lbl_1_bss_8B3A0[332];

u32 fn_1_3FC9C(void) {
    s16 value = *(s16*)(lbl_1_bss_8B3A0 + 8);
    return (u32)value & 0xff;
}
