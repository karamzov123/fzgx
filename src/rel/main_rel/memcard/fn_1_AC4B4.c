#include "types.h"

typedef struct Fn1AC4B4Target {
    u8 pad_00[0x24];
    struct Fn1AC4B4State *state;
} Fn1AC4B4Target;

typedef struct Fn1AC4B4State {
    u8 pad_00[0x2];
    u16 value;
} Fn1AC4B4State;

void fn_1_AC4B4(Fn1AC4B4Target *target) {
    target->state->value = 300;
}
