#include "types.h"

extern void *fn_1_48730(void);

u16 fn_1_48690(void) {
    void *value = fn_1_48730();
    if (value != 0) {
        return *(u16 *)((u8 *)value + 8);
    }
    return 8;
}
