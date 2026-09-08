#include "types.h"

typedef struct MovieEntry {
    u32 field_0;
    u32 field_4;
    u32 field_8;
} MovieEntry;

typedef struct MovieObject {
    u8 pad_0000[0x1308];
} MovieObject;

void fn_12_CA98(MovieObject *self, s32 index, u32 value_0, u32 value_4, u32 value_8) {
    MovieEntry *entry = (MovieEntry *)((u8 *)self + index * 12 + 0x1308);
    entry->field_0 = value_0;
    entry->field_4 = value_4;
    entry->field_8 = value_8;
}
