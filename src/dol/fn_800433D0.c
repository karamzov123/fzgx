#include "types.h"

void fn_800433D0(void* self, s32 val) {
    *(s32*)((u8*)self + 0xB4) = val;
    *(s32*)((u8*)self + 0xB8) = val / 96;
}
