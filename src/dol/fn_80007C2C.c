#include "types.h"

extern u32 fn_800744F8(void *, u32);
extern u32 lbl_801A6408;

void fn_80007C2C(void) {
    u32 loc_8[2];
    /* frame */
    loc_8[0] = lbl_801A6408;
    fn_800744F8(loc_8, (0x1000000 - 1));
}
