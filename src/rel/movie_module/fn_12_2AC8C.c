#include "types.h"

struct MovieModule {
    char pad_1b30[0x1b30];
    struct MovieState *state;
};

struct MovieState {
    char pad_7c[0x7c];
    int value;
};

int fn_12_2AC8C(struct MovieModule *module) {
    return module->state->value;
}
