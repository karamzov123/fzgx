#include "types.h"

extern u32 lbl_1_bss_8E428;
extern void fn_1_149CA4(u32 arg0, u32 arg1, u32 arg2, u32 arg3);

// Load four values from BSS table and pass to fn_1_149CA4.
void fn_1_149BF4(void) {
    u8* base = (u8*)&lbl_1_bss_8E428;
    u32 arg0 = *(u32*)(base + 0x4);
    u32 arg1 = *(u32*)(base + 0x14);
    u32 arg2 = *(u32*)(base + 0x8);
    u32 arg3 = *(u32*)(base + 0x18);
    fn_1_149CA4(arg0, arg1, arg2, arg3);
}
