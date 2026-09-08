#include "rel/main_rel/globals.h"

typedef struct {
    u32 flags;
    u8 pad_04[0x3c];
} RankingEntry;

// Set the high flag on each ranking entry managed by the singleton.
void fn_1_15B544(void) {
    s16 i;
    RankingEntry *entry;

    i = 0;
    entry = (RankingEntry *)lbl_1_bss_3BE0->unk_54;
    while (i < (s32)lbl_1_bss_3BE0->unk_48) {
        entry->flags |= 0x80000000u;
        i++;
        entry = (RankingEntry *)((u8 *)entry + 0x40);
    }
}
