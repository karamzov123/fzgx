#include "types.h"

extern int fn_12_2F1B4(u8 *base, int index);

void fn_12_2D7DC(u8 *base, int index, int value) {
    int allowed;

    if (index == 6 && value == 1 && fn_12_2F1B4(base, 3) == 0) {
        allowed = 0;
    } else if (index == 5 && value == 1 && fn_12_2F1B4(base, 2) == 0) {
        allowed = 0;
    } else {
        allowed = 1;
    }

    if (allowed != 0) {
        u8 *entry = base;
        entry += index << 2;
        *(int *)(entry + 0x9a0) = value;
    }
}
