#include "types.h"

extern u32 OSDisableInterrupts(void);
extern u32 OSRestoreInterrupts(u32);
extern u32 __LCEnable(u32);

u32 LCEnable(void) {
    u32 t0;
    t0 = OSDisableInterrupts();
    __LCEnable(t0);
    return OSRestoreInterrupts(t0);
}
