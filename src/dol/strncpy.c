#include "types.h"

char *strncpy(char *dst, const char *src, size_t n) {
    const unsigned char *s = (const unsigned char *)src - 1;
    unsigned char *d = (unsigned char *)dst - 1;
    n++;

    while (--n != 0) {
        if ((*++d = *++s) == 0) {
            while (--n != 0) {
                *++d = 0;
            }
            return dst;
        }
    }

    return dst;
}
