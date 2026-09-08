#include "types.h"

extern void fn_1_103264(void *entry, void *arg);

typedef struct Entry {
    u8 data[0xac];
} Entry;

typedef struct Object {
    u8 pad_84[0x84];
    s32 count;
    Entry entries[1];
} Object;

void fn_1_150ED0(Object *obj, void *arg) {
    s32 count;
    Entry *entry;

    count = obj->count;
    entry = obj->entries;
    while (count > 0) {
        fn_1_103264(entry, arg);
        count -= 1;
        entry += 1;
    }
}
