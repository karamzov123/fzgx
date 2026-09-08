#include "types.h"

extern u32 OSDisableInterrupts(u32);
extern u32 OSRestoreInterrupts(u32);
extern u32 lbl_801A6934;

u32 fn_8001A78C(u32 arg0) {
    u32 v0;
    u32 t0;
    v0 = lbl_801A6934;
    t0 = OSDisableInterrupts(arg0);
    lbl_801A6934 = arg0;
    OSRestoreInterrupts(t0);
    return v0;
}
