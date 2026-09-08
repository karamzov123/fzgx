#include "rel/main_rel/globals.h"

// Clears the high bit of each ranking entry flag.
void fn_1_15B588(void) {
    u8 *entry;
    s16 i;
    Obj_1_data_2A7E0_At3C *obj;

    obj = lbl_1_data_2A7E0.unk_3C;
    entry = (u8 *)obj;
    i = 0;
    while (i < obj->unk_5E) {
        u32 *value = *(u32 **)(entry + 0xc4);

        entry += 4;
        i++;
        *value &= 0x7fffffff;
    }
}
