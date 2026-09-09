#include "types.h"

void* __memrchr(const void* src, s32 val, u32 n) {
    const u8* p;
    u32 value = (u32)val & 0xFF;
    for (p = (const u8*)src + n, n++; --n;) {
        if (*--p == value) {
            return (void*)p;
        }
    }
    return 0;
}
