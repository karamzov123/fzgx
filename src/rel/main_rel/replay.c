#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_F2CD8 */
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
/* fzgx:end fn_1_F2CD8 */

/* fzgx:begin fn_1_F2EFC noprologue */
#include "types.h"
extern u8 lbl_1_bss_7EFD4;
extern u8 lbl_1_bss_7EFD8[0x44];

void fn_1_F2EFC(void) {
    u8 flags = lbl_1_bss_7EFD4;

    if ((flags & 1) == 0) {
        return;
    }
    if (((flags >> 5) & 1) != 0) {
        return;
    }

    lbl_1_bss_7EFD4 = flags & ~8;
    lbl_1_bss_7EFD8[0x19] = 1;
}
/* fzgx:end fn_1_F2EFC */
