#include "types.h"

extern u8 lbl_1_bss_8F428[320];
extern f32 lbl_1_rodata_D8C8[18];
extern void fn_80008BEC(void *, int, u32);
extern void fn_1_1568C4(void *);

typedef struct Entry {
    u32 unk_00;
    void *owner;
    s32 unk_08;
    s32 unk_0c;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1c;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s32 unk_2c;
    u32 unk_30;
    f32 value_34;
    u8 unk_38[0x18];
} Entry;

void fn_1_15665C(void *owner, s32 index) {
    Entry *entry;

    entry = (Entry *)lbl_1_bss_8F428 + index;
    fn_80008BEC(entry, 0, 0x50);
    entry->unk_00 = 0;
    entry->owner = owner;
    entry->unk_08 = -1;
    entry->unk_0c = -1;
    entry->unk_10 = -1;
    entry->unk_14 = -1;
    entry->unk_1c = -1;
    entry->unk_20 = -1;
    entry->unk_18 = -1;
    entry->unk_24 = -1;
    entry->unk_28 = -1;
    entry->unk_2c = -1;
    entry->value_34 = lbl_1_rodata_D8C8[0];
    fn_1_1568C4(entry);
}
