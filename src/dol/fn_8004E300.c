#include "types.h"

struct Fn8004E300 {
    u8 pad_0[0x28];
    s16 field_28;
    s16 field_2A;
    s16 field_2C;
    s16 field_2E;
};

void fn_8004E300(struct Fn8004E300* dst, s16* value_1, s16* value_2) {
    dst->field_28 = value_1[0];
    dst->field_2A = value_2[0];
    dst->field_2C = value_1[1];
    dst->field_2E = value_2[1];
}
