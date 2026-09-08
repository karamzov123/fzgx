#include "types.h"

typedef struct MovieModule {
    u8 pad_0000[0x1b30];
    struct MovieState *state;
} MovieModule;

typedef struct MovieState {
    u8 pad_0000[0x7c];
    int field_007c;
} MovieState;

void fn_12_2AC98(MovieModule *module) {
    module->state->field_007c = 1;
}
