#include "types.h"

extern u32 DummyCommandBlock_8015CE80[];
extern u32 FatalErrorFlag_801A68D8;
extern u32 OSDisableInterrupts(void);
extern u32 OSRestoreInterrupts(u32);
extern u32 PausingFlag_801A68D0;
extern u32 executing_801A68C0;

s32 DVDGetDriveStatus(void) {
    u32 v0;
    u32 v1;
    u32 v2;
    u32 t0, t1, t2;
    t0 = OSDisableInterrupts();
    v0 = (t0 + 0);
    v2 = t0;
    if ((s32)FatalErrorFlag_801A68D8 != 0) {
    v1 = -1;
    } else {
    if ((s32)PausingFlag_801A68D0 != 0) {
    v1 = 8;
    } else {
    v1 = executing_801A68C0;
    if (v1 == 0) {
    v1 = 0;
    } else {
    if (v1 == (u32)&DummyCommandBlock_8015CE80) {
    v1 = 0;
    } else {
    t1 = OSDisableInterrupts();
    v2 = t1;
    v1 = *(u32 *)((u8 *)v1 + 12);
    if ((s32)v1 == 3) {
    v1 = 1;
    }
    t2 = OSRestoreInterrupts(v2);
    v2 = t2;
    }
    }
    }
    }
    OSRestoreInterrupts(v0);
    return v1;
}
