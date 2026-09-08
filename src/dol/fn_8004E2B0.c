#include "types.h"

typedef struct {
    u8 _pad[0x34];
    s16 field_34;
    s16 field_36;
    s16 field_38;
} Fn8004E2B0Data;

void fn_8004E2B0(Fn8004E2B0Data* data, s16* value_1, s16* value_2, s16* value_3) {
    *value_1 = data->field_34;
    *value_2 = data->field_36;
    *value_3 = data->field_38;
}
