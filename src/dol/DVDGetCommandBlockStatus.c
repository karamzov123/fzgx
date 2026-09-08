#include "types.h"

extern u32 OSDisableInterrupts(u32);
extern u32 OSRestoreInterrupts(void);

u32 DVDGetCommandBlockStatus(u32 arg0) {
    u32 v0;
    u32 v1;
    OSDisableInterrupts(arg0);
    v0 = *(u32 *)((u8 *)arg0 + 12);
    v1 = arg0;
    if ((s32)v0 == 3) {
    v1 = 1;
    } else {
    v1 = v0;
    }
    OSRestoreInterrupts();
    return v1;
}
