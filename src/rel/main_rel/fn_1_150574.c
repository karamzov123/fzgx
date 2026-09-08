#include "types.h"

extern u32 lbl_1_data_2A7E0[62];
extern void fn_1_150CEC(u32 arg);
extern void fn_1_151668(u32 arg);

void fn_1_150574(void) {
    u32 value = *(u32*)((u8*)lbl_1_data_2A7E0 + 0x3c);
    fn_1_150CEC(value);
    fn_1_151668(value);
}
