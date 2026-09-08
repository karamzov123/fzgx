#include "types.h"

extern void fn_1_103264(void *, void *);

typedef struct Entry {
    u8 unk00[0xac];
} Entry;

typedef struct Container {
    s32 count;
    Entry entries[1];
} Container;

void fn_1_DC5E8(Container *container, void *arg) {
    s32 count;
    Entry *entry;

    entry = container->entries;
    count = container->count;
    while (count > 0) {
        fn_1_103264(entry, arg);
        count--;
        entry++;
    }
}
