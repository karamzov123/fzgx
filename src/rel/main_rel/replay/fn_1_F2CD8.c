#include "types.h"

extern u32 lbl_1_bss_7EFD8[17];

void fn_1_F2CD8(u32 value, u32 *cursor, u32 index) {
    s32 target;
    u8 *p;
    u8 *end;

    target = index | 4;
    end = (u8 *)lbl_1_bss_7EFD8[16] + 0xa4;
    *cursor = value;

    while (*cursor >= (u32)end) {
        p = (u8 *)*cursor;
        if ((p[0] & 7) == (target & 0xff)) {
            break;
        }
        *cursor -= 7;
    }

    if (*cursor >= (u32)end) {
        return;
    }
    *cursor = value;
}
