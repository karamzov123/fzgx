#include "types.h"

void* memchr(const void* src, s32 val, u32 n) {
    const u8* p;
    u32 value = (u32)val & 0xFF;
    for (p = (const u8*)src - 1, n++; --n;) {
        if ((*++p & 0xFF) == value) {
            return (void*)p;
        }
    }
    return 0;
}
