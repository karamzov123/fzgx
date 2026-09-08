#include "types.h"

extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8AD4(u32 value) {
    lbl_1_bss_7F0C0[4672] |= (0x8000u << 16) >> ((value & 0xff) + 0x10);
}
