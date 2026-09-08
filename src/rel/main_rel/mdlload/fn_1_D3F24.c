#include "types.h"

extern void fn_1_103090(void *ptr);

void fn_1_D3F24(void *base) {
    s32 value;
    s32 count;
    char *ptr;

    value = 1;
    ptr = (char *)base + 0x10000;
    count = *(s32 *)(ptr - 0x1820);
    ptr -= 0x181c;

    while (count > 0) {
        *(s32 *)(ptr + 0x68) = value;
        fn_1_103090(ptr);
        count--;
        ptr += 0xac;
    }
}
