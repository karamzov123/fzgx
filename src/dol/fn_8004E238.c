#include "types.h"

typedef struct {
    u8 _pad[0x0c];
    s32 field_0c;
    u8 _pad_10[0x04];
    s32 field_14;
    u32 field_18;
    u32 field_1c;
    u32 field_20;
    s32 field_24;
} Fn8004E238Data;

s32 fn_8004E238(Fn8004E238Data* data, u32 arg_1, u32 arg_2, u32 arg_3) {
    if (data->field_0c == 0) {
        data->field_14 = 1;
        data->field_18 = arg_1;
        data->field_1c = arg_2;
        data->field_20 = arg_3;
        data->field_24 = 0;
        return 1;
    }
    return 0;
}
