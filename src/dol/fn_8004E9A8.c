#include "types.h"

struct fn_8004E9A8_State {
    u8 pad_0[0x10];
    s32 count_10;
    u8 pad_14[0xC94];
    s32 field_CA8;
    s32 field_CAC;
};

extern struct fn_8004E9A8_State lbl_8017E980;

void fn_8004E9A8(void) {
    struct fn_8004E9A8_State* state = &lbl_8017E980;
    while (state->field_CAC == 1) {
        state->count_10++;
    }
    state->field_CA8 = 1;
}
