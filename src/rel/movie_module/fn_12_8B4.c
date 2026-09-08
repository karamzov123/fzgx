#include "types.h"

typedef struct Fn12_8B4Data {
    u8 unk_00[8];
    s32 value_08;
    s32 value_0C;
} Fn12_8B4Data;

void fn_12_8B4(Fn12_8B4Data *data, s32 *value_08, s32 *value_0C) {
    *value_08 = data->value_08;
    *value_0C = data->value_0C;
}
