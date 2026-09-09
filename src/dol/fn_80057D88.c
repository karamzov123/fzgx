#include "types.h"

s32 fn_80057D88(void* p, s32 index, s32 offset) {
    index <<= 3;
    offset <<= 2;
    p = (u8*)p + index;
    p = (u8*)p + offset;
    return *(s32*)((u8*)p + 0x28);
}
