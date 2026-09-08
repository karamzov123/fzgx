#include "types.h"

extern void fn_1_103090(void *entry);

typedef struct Entry {
    u8 pad[0x68];
    u32 active;
    u8 tail[0x40];
} Entry;

typedef struct Object {
    u8 pad_84[0x84];
    s32 count;
    Entry entries[1];
} Object;

void fn_1_150C8C(Object *obj) {
    s32 count;
    Entry *entry;

    count = obj->count;
    entry = obj->entries;
    while (count > 0) {
        entry->active = 1;
        fn_1_103090(entry);
        count -= 1;
        entry += 1;
    }
}
