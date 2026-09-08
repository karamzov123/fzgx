#include "types.h"

extern u8 lbl_8015B200[256];

s32 fn_8007ED90(s32 value) {
    if (value == -1) {
        return -1;
    }
    return lbl_8015B200[(u8)value];
}
