#include "types.h"

// fn_1_8C5AC: Load float from indexed structure

extern u32 lbl_1_bss_6D838;

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
