#include "types.h"

void fn_12_21D40(void *base, int index, int value) {
    u8 *p = (u8 *)base;
    int offset = index * 0x74;
    p += offset;
    *(int *)(p + 0x114c) = value;
}
