#include "types.h"

extern u32 fn_800791E8(u32, u32);
extern u32 fn_800794F0(u32, u32);
extern u32 memcpy(u32, u32, u32);

void fn_80008BA8(u32 arg0, u32 arg1, u32 arg2) {
    u32 v0;
    v0 = (arg1 | arg0);
    if ((v0 & 0x3) != 0) {
    memcpy(arg0, arg1, arg2);
    } else {
    if ((v0 & 0x1F) != 0) {
    fn_800794F0(arg0, arg1);
    } else {
    fn_800791E8(arg0, arg1);
    }
    }
}
