#include "types.h"

extern u8 lbl_1_data_1C5DC[4];
extern u8 lbl_1_data_1C5EC[100];

void *fn_1_541A8(void *arg, s32 value) {
    s32 special;
    u32 remainder;
    s32 index;
    u32 *row;

    if (arg == 0) {
        return lbl_1_data_1C5DC;
    }

    remainder = (u32)arg - ((u32)arg / 100) * 100;
    special = 0;
    if (remainder >= 10 && remainder <= 20) {
        special = 1;
    }

    if (special != 0) {
        index = 3;
    } else {
        arg = (void *)(remainder - (remainder / 10) * 10);
        special = 0;
        if ((u32)arg >= 4 || arg == 0) {
            special = 1;
        }
        index = 3;
        if (special == 0) {
            index = (u32)arg - 1;
        }
    }

    row = (u32 *)(lbl_1_data_1C5EC + index * 24);
    return (void *)row[value < 0 ? 0 : (value > 5 ? 5 : value)];
}
