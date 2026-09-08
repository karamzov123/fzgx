#include "types.h"

u32 fn_12_2D73C(u8 *base, int index) {
    u8 *entry = base;
    entry += index << 2;
    return *(u32 *)(entry + 0x9a0);
}
