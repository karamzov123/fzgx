#include "types.h"

extern void fn_80008BEC(void *dst, void *value, u32 size);
extern void fn_80008BA8(void *dst, void *value, u32 size);

void fn_1_E5A50(void *base, void *value) {
    if (value == NULL) {
        fn_80008BEC((void *)((char *)base + 0x38), NULL, 0xC);
    } else {
        fn_80008BA8((void *)((char *)base + 0x38), value, 0xC);
    }
}
