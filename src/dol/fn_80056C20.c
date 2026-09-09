#include "types.h"

extern u32 fn_800565FC(u32, ...);
extern char lbl_80092188[];

s32 fn_80056C20(void* p) {
    if (p == 0) {
        fn_800565FC((u32)&lbl_80092188);
        return -1;
    }
    return *(s8*)((u8*)p + 1);
}
