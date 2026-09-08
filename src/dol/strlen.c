#include "types.h"

size_t strlen(const char *str) {
    const unsigned char *p = (const unsigned char *)str - 1;
    size_t len = (size_t)-1;

    do {
        len++;
    } while (*++p != 0);

    return len;
}
