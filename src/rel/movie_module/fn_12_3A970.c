#include "types.h"

extern u32 fn_12_38DBC(u32);

void fn_12_3A970(u32 arg0) {
    u32 t0;
    t0 = fn_12_38DBC(arg0);
    *(u32 *)((u8 *)t0 + 84) = arg0;
}
