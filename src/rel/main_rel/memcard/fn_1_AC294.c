#include "types.h"

typedef struct Fn1AC294State {
    u8 pad_00[0x2];
    u16 value;
} Fn1AC294State;

typedef struct Fn1AC294Target {
    u8 id;
    u8 pad_01[0x13];
    void *arg;
    u8 pad_18[0xC];
    Fn1AC294State *state;
} Fn1AC294Target;

extern void fn_8002FF80(u8 id, void *arg, void *data, int zero);

void fn_1_AC294(Fn1AC294Target *target) {
    target->state->value = 300;
    fn_8002FF80(target->id, target->arg, (u8 *)target->state + 0x20, 0);
}
