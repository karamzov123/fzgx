#include "types.h"

typedef struct FnEntry {
    u8 padding[8];
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    s32 field_18;
} FnEntry;

void fn_12_2E2DC(s32 value, FnEntry *entry, s32 *result, s32 *out) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    a = entry->field_0c * 1440000;
    b = entry->field_08 * 86400000;
    c = entry->field_10 * 24000;
    d = (entry->field_14 + entry->field_18) * 1000;
    *result = a + b + c + d;
    *out = 24000;
    *out = value;
}
