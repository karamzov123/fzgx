#include "types.h"

typedef struct Fn8004A550Data {
    u8 _pad14[0x14];
    s32 field14;
    s32 field18;
} Fn8004A550Data;

s32 fn_8004A550(Fn8004A550Data *data, s32 value4, s32 value5) {
    data->field18 = value4;
    data->field14 = value5;
    return 1;
}
