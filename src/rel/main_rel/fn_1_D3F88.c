#include "types.h"

extern void fn_1_1030A4(void *ptr);

// Iterate through items in a structure, calling fn_1_1030A4 on each.
void fn_1_D3F88(void *base) {
    s32 count;
    char *ptr;
    
    ptr = (char *)base + 0x10000;
    count = *(s32 *)(ptr - 0x1820);
    ptr -= 0x181c;
    
    while (count > 0) {
        fn_1_1030A4(ptr);
        count--;
        ptr += 0xac;
    }
}
