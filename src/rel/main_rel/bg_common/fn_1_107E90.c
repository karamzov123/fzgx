#include "types.h"

extern u8 lbl_1_data_3FFBC[1396];
extern void lbl_8006DBAC(void *arg);
extern void lbl_8006E1B0(void *arg0, void *arg1);

typedef struct {
    u8 unk0[0x8];
    u32 index;
    u8 *data;
    u8 unk10[0x8];
    u32 count;
    u8 unk1c[0x8];
    u8 *entries;
} Fn107E90Context;

void fn_1_107E90(Fn107E90Context *context) {
    u32 i;
    u8 *entries;
    u8 *base;
    u8 *entry;

    i = 0;
    entries = context->entries;
    base = *(u8 **)(*(u8 **)(context->data + 0x150) + 0x8);
    entry = base + ((u32 *)lbl_1_data_3FFBC)[context->index] * 0x18c;
    for (; i < context->count; i++, entries += 0x44) {
        lbl_8006DBAC(entry + 0x88);
        lbl_8006E1B0(entries + 0x28, entries + 0x10);
    }
}
