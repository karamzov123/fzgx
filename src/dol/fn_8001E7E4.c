#include "types.h"

extern u32 lbl_801A69E0;
extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32 state);

u32 fn_8001E7E4(u32 value) {
    u32 previous = lbl_801A69E0;
    u32 state = OSDisableInterrupts();
    lbl_801A69E0 = value;
    OSRestoreInterrupts(state);
    return previous;
}
