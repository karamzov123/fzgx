#include "types.h"

extern u32 __OSMaskInterrupts(u32);
extern u32 __OSSetInterruptHandler(u32, u32);
extern u32 __OSUnmaskInterrupts(u32);
extern void fn_8008ECE0(void);

u32 fn_8008ED30(void) {
    __OSMaskInterrupts((0x10000 + -32768));
    __OSSetInterruptHandler(25, (u32)fn_8008ECE0);
    return __OSUnmaskInterrupts(64);
}
