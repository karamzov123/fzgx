#include "types.h"

extern u32 OSDisableInterrupts(void);
extern u32 OSRestoreInterrupts(u32);

u32 ARGetDMAStatus(void) {
    u32 v0;
    u32 t0;
    t0 = OSDisableInterrupts();
    v0 = (*(u16 *)0xCC00500A & 0x200);  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
    OSRestoreInterrupts(t0);
    return v0;
}
