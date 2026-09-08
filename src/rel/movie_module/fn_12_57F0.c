#include "types.h"

extern void *memcpy(void *dst, const void *src, u32 size);

void *fn_12_57F0(void *dst, const void *src, u32 size) {
    return memcpy(dst, src, size);
}
