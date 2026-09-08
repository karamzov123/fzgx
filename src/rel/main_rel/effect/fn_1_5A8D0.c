#include "types.h"

typedef struct {
    u8 pad[0x10];
    s32 field_10;
} Fn1_5A8D0Data;

void fn_1_5A8D0(Fn1_5A8D0Data *data) {
    data->field_10 = 2;
}
