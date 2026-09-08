#include "types.h"

extern u32 memset(u32, u32, u32);

void fn_80057B5C(u32 arg0) {
    if (arg0 != 0) {
    memset(arg0, 0, 36);
    *(u32 *)((u8 *)arg0 + 4) = 0;
    }
}
