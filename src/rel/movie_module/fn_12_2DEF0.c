#include "types.h"

typedef struct MovieEntry {
    u8 pad_0[0x14];
    s32 value_14;
    s32 value_18;
} MovieEntry;

extern int fn_12_2DEB0(void *arg0, s32 value_14, s32 value_18);

int fn_12_2DEF0(void *arg0, MovieEntry *entry) {
    if (entry == 0) {
        return 0;
    }
    return fn_12_2DEB0(arg0, entry->value_14, entry->value_18);
}
