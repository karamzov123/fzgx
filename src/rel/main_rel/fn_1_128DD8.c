#include "types.h"

extern u8 lbl_1_data_405C0[8];

u8 fn_1_128DD8(u8 value) {
    u8 i;

    i = 0;
    while (i < 6) {
        if (lbl_1_data_405C0[i] == value) {
            return i;
        }
        i++;
    }
    return i;
}
