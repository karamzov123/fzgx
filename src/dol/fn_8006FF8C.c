#include "types.h"

extern u32 lbl_801A6CE0;

void fn_8006FF8C(s32 value) {
    if (value != 0 || ((value = lbl_801A6CE0) & 0x10) != 0) {
        lbl_801A6CE0 |= 0x20;
    } else {
        lbl_801A6CE0 = value & ~0x20;
    }
}
