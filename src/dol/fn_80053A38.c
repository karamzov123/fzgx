#include "types.h"

extern u32 memset(u32, u32, u32);
extern u32 lbl_80187380[];

void fn_80053A38(void* p) {
    s32 index = *(s8*)((u8*)p + 1);
    memset((u32)p, 0, 0x2014);
    lbl_80187380[index] = 0;
}
