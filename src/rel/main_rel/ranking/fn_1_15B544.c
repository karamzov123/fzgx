#include "types.h"

typedef struct {
    u8 pad_00[0x48];
    s32 count;
    u8 pad_4c[0x08];
    u32 *entries;
} Manager;

typedef struct {
    u32 flags;
    u8 pad_04[0x3c];
} Entry;

// The volatile qualifier preserves the retail reload of the singleton each iteration.
extern Manager * volatile lbl_1_bss_3BE0;

void fn_1_15B544(void) {
    s16 i;
    Entry *entry;

    i = 0;
    entry = (Entry *)lbl_1_bss_3BE0->entries;
    while (i < lbl_1_bss_3BE0->count) {
        entry->flags |= 0x8000u << 16;
        i++;
        entry = (Entry *)((u8 *)entry + 0x40);
    }
}
