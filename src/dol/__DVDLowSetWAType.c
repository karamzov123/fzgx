#include "types.h"

extern u32 OSDisableInterrupts(u32, u32);
extern u32 OSRestoreInterrupts(u32);
extern u32 WorkAroundSeekLocation_801A6880;
extern u32 WorkAroundType_801A687C;

void __DVDLowSetWAType(u32 arg0, u32 arg1) {
    u32 t0;
    t0 = OSDisableInterrupts(arg0, arg1);
    WorkAroundType_801A687C = (arg0 + 0);
    WorkAroundSeekLocation_801A6880 = (arg1 + 0);
    OSRestoreInterrupts(t0);
}
