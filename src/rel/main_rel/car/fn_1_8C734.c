#include "rel/main_rel/globals.h"

// Return the selected car's float field, or the null object's field when unavailable.
f32 fn_1_8C734(u32 index) {
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
    return *(f32 *)(ptr + 0xf8);
}
