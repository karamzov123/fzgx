#include "types.h"

typedef struct {
    u8 pad0[4];
    u32 field4;
    u32 field8;
} Fn800284E8Data;

void fn_800284E8(Fn800284E8Data *data) {
    data->field4 = 5;
    data->field8 = 0;
}
