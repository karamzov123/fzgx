#include "rel/main_rel/globals.h"

void fn_1_F2CD8(u8 *value, u8 **cursor, u32 index) {
    s32 target;
    Obj_1_bss_7EFD8_At40 *entries_end;
    u8 *entry;

    // Move backward through replay entries until the requested flag or boundary is found.
    target = (index | 4) & 0xff;
    entries_end = lbl_1_bss_7EFD8.unk_40 + 1;
    *cursor = value;

    while (*cursor >= (u8 *)entries_end) {
        entry = *cursor;
        if ((entry[0] & 7) == target) {
            break;
        }
        *cursor -= 7;
    }

    if (*cursor < (u8 *)entries_end) {
        *cursor = value;
    }
}
