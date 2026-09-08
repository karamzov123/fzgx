#include "types.h"

extern u32 lbl_12_bss_1B75C[5];

typedef struct MovieEntry {
    s32 used;
    u32 arg0;
    u32 arg1;
    u32 unk_0c;
} MovieEntry;

MovieEntry *fn_12_3310C(u32 arg0, u32 arg1) {
    MovieEntry *entry;
    s32 count;
    MovieEntry *last;
    s32 index;
    s32 i;

    count = (s32)lbl_12_bss_1B75C[0];
    last = 0;
    index = (s32)lbl_12_bss_1B75C[1];
    entry = (MovieEntry *)lbl_12_bss_1B75C[2];
    if (index >= count) {
        return 0;
    }

    for (i = 0; i < count; i++) {
        last = entry;
        if (entry->used == 0) {
            break;
        }
        entry++;
    }

    last->used = 1;
    last->arg0 = arg0;
    last->arg1 = arg1;
    lbl_12_bss_1B75C[1]++;
    return last;
}
