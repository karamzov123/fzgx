#include "types.h"

extern u32 lbl_801A6638;

u32 fn_80082908(void) {
    u32 v0;
    v0 = ((lbl_801A6638 * (0x41C60000 + 20077)) + 12345);
    lbl_801A6638 = v0;
    return ((v0 >> 16) & 0x7FFF);
}
