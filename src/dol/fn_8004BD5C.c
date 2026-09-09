#include "types.h"

extern u32 fn_800474E4(u32);
extern u8 lbl_80090A80[];

s32 fn_8004BD5C(void* p) {
    if (p == 0) {
        fn_800474E4((u32)&lbl_80090A80);
        return -1;
    }
    return *(s8*)((u8*)p + 0x71);
}
