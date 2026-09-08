#include "types.h"

extern u32 GXInvalidateTexAll(u32);
extern u32 fn_80035C50(u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 lbl_801A63D0;

void fn_80005AD0(u32 arg0) {
    u32 t0;
    t0 = fn_80035C50(arg0, lbl_801A63D0, 640, 480, 14, 0, 0, 0);
    GXInvalidateTexAll(t0);
}
