#include "types.h"

extern u32 lbl_801A6D98;

void fn_80077BBC(s32 mode) {
    s32 value = 0;
    if (mode) {
        value = 3;
    }
    lbl_801A6D98 = value;
}
