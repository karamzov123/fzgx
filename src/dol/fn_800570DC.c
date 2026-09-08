#include "types.h"

extern u32 fn_80056E9C(u32, u32, u32, u32, u32);

void fn_800570DC(u32 arg0, u32 arg1) {
    fn_80056E9C(arg0, arg1, 0, 0, (0x100000 - 1));
}
