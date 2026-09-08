#include "types.h"

extern void fn_1_103090(void *);

typedef struct Entry {
    u8 unk00[0x68];
    s32 initialized;
    u8 unk6c[0x40];
} Entry;

typedef struct Container {
    s32 count;
    Entry entries[1];
} Container;

void fn_1_DC3A4(Container *container) {
    s32 count;
    Entry *entry;

    count = container->count;
    entry = container->entries;
    while (count > 0) {
        entry->initialized = 1;
        fn_1_103090(entry);
        count--;
        entry++;
    }
}
