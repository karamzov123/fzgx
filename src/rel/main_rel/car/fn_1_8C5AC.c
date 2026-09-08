#include "rel/main_rel/globals.h"

// Return the indexed car's float value.
f32 fn_1_8C5AC(u32 index) {
    u32 base;
    u32 addr;
    u32 ptr;

    base = lbl_1_bss_6D838;
    if (base != 0) {
        addr = base + index * 0x620;
    } else {
        addr = 0;
    }

    ptr = *(u32 *)(addr + 0x49c);
    return *(f32 *)(ptr + 0x68);
}
