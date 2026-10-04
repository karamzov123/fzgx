#include "types.h"
#include "dolphin/types.h"

extern u32 OSDisableInterrupts(void);
extern u32 OSRestoreInterrupts(u32);
extern volatile u32 *lbl_801A6678; /* shared interrupt state is hardware-updated */
extern volatile vu32 __EXIRegs[]; /* hardware register block: reads have device effects */

void fn_8008E9B4(void) {
    u32 tmp_call2;
    s32 count;
    s32 i;
    u32 interrupts;
    u32 src;
    u8 *dst;

    while (lbl_801A6678[3] & 4) {
        if (__EXIRegs[13] & 1) {
            continue;
        }
        interrupts = OSDisableInterrupts();
        if (lbl_801A6678[3] & 3) {
            if (lbl_801A6678[3] & 2) {
                count = lbl_801A6678[4];
                if (count != 0) {
                    dst = (u8 *)lbl_801A6678[5];
                    /* fzgx-allow: S2 memory-mapped register */
                    src = *(volatile vu32 *)0xCC006838; /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
                    for (i = 0; i < count; i++) {
                        *dst++ = (u8)(src >> ((3 - i) * 8));
                    }
                }
            }
            lbl_801A6678[3] &= ~3;
        }
        tmp_call2 = OSRestoreInterrupts(interrupts);
        tmp_call2;
        break;
    }
}
