#include "types.h"

extern u32 OSDisableInterrupts(u32);

void fn_8008E73C(u32 arg0) {
    u32 t0;
    t0 = OSDisableInterrupts(arg0);
    *(u32 *)((u8 *)arg0 + 0) = t0;
}
