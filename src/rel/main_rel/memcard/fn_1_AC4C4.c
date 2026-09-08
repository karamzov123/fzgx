#include "types.h"

typedef struct Fn1AC4C4State {
    u16 pad_00;
    u16 value_02;
    s32 result;
    u32 flags;
    u8 pad_0c[0x4];
    u8 data_10[0x4];
    u8 data_14[0x4];
} Fn1AC4C4State;

typedef struct Fn1AC4C4Target {
    u8 value;
    u8 pad_01[0x23];
    Fn1AC4C4State *state;
} Fn1AC4C4Target;

extern s32 fn_8002A988(u8 value, void *arg1, void *arg2);

void fn_1_AC4C4(Fn1AC4C4Target *target) {
    target->state->result =
        fn_8002A988(target->value, &target->state->data_10, &target->state->data_14);
    if (target->state->result == 0) {
        target->state->flags |= 0x80;
    }
    if (target->state->result != -1) {
        target->state->value_02 = 0;
    }
}
