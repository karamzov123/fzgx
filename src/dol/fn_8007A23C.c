#include "types.h"

extern u32 fn_8007A2E8(u32, u32);
extern u32 fn_8007A710(u32, u32);

void fn_8007A23C(u32 arg0, u32 arg1) {
    u32 v0;
    u32 v1;
    if (arg1 != 0) {
    v0 = *(u32 *)((u8 *)arg1 + -4);
    v1 = v0;
    if ((v0 & 0x1) == 0) {
    v1 = *(u32 *)((u8 *)v1 + 8);
    } else {
    v1 = (*(u32 *)((u8 *)arg1 + -8) & ~0x7);
    v1 -= 8;
    }
    if (v1 <= 68) {
    fn_8007A2E8(arg0, arg1);
    } else {
    fn_8007A710(arg0, arg1);
    }
    }
}
