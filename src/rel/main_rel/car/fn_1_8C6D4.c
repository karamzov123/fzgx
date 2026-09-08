#include "rel/main_rel/globals.h"

u32 fn_1_8C6D4(u32 index) {
    u32 base;
    u32 car;
    u32 state;

    // Return the requested car's value from its state block.
    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        car = base + index * 0x620;
    } else {
        car = 0;
    }

    state = *(u32 *)(car + 0x49c);
    return *(u32 *)(state + 0xb8);
}
