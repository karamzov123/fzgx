#include "types.h"

extern u32 __OSMaskInterrupts(u32);
extern u32 __OSSetInterruptHandler(u32, u32);
extern u32 __OSUnmaskInterrupts(u32);
extern u32 lbl_801A6E2C;
extern void fn_8008F8B0(void);
extern void fn_8008F8F0(void);

u32 fn_8008F7E4(void) {
    __OSMaskInterrupts((0x20000 + -32768));
    __OSMaskInterrupts(64);
    lbl_801A6E2C = (u32)fn_8008F8F0;
    __OSSetInterruptHandler(25, (u32)fn_8008F8B0);
    return __OSUnmaskInterrupts(64);
}
