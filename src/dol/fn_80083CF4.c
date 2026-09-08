#include "types.h"

char *fn_80083CF4(char *dest, const char *src, size_t n) {
    const u8 *s = (const u8 *)src - 1;
    u8 *d = (u8 *)dest - 1;
    u8 c;

    while (*++d != 0) {
    }
    --d;
    ++n;
    while (--n != 0) {
        c = *++s;
        *++d = c;
        if (c == 0) {
            --d;
            break;
        }
    }
    *++d = 0;
    return dest;
}
