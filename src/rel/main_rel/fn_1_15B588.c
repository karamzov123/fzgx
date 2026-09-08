#include "types.h"

extern u32 lbl_1_data_2A7E0[62];

void fn_1_15B588(void) {
    u8 *entry;
    s16 i;
    u8 *obj;

    obj = (u8 *)lbl_1_data_2A7E0[15];
    entry = obj;
    i = 0;
    while (i < *(u8 *)(obj + 0x5e)) {
        u32 *value = *(u32 **)(entry + 0xc4);
        entry += 4;
        i++;
        *value &= 0x7fffffff;
    }
}
