#include "types.h"

typedef struct MovieModuleEntry {
    u8 padding[0xcc0];
    s32 value;
} MovieModuleEntry;

void fn_12_2E41C(MovieModuleEntry *base, s32 value, s32 index) {
    base = (MovieModuleEntry *)((u8 *)base + (index << 2));
    base->value = value;
}
