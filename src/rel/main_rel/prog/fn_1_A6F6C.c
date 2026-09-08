#include "types.h"

extern s16 lbl_1_bss_962;
extern u32 lbl_1_data_341B8;

s32 fn_1_A6F6C(void) {
    s32 result = 0;

    if (lbl_1_bss_962 == 0x99) {
        if (*(s8 *)((u8 *)lbl_1_data_341B8 + 4) == 5) {
            result = 1;
        }
    }

    return result;
}
