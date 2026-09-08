#include "types.h"

extern u32 OSDisableInterrupts(u32);
extern u32 OSRestoreInterrupts(u32);
extern u32 lbl_801A6C04;

u32 fn_800321D8(u32 arg0) {
    u32 v0;
    u32 t0;
    v0 = lbl_801A6C04;
    t0 = OSDisableInterrupts(arg0);
    lbl_801A6C04 = arg0;
    OSRestoreInterrupts(t0);
    return v0;
}
