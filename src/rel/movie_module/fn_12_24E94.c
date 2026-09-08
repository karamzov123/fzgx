#include "types.h"

typedef struct MovieModule MovieModule;
typedef struct MovieState MovieState;

struct MovieModule {
    u8 pad_0000[0x1aec];
    MovieState *state;
};

struct MovieState {
    u8 pad_0000[0x14];
    u32 value;
};

u32 fn_12_24E94(MovieModule *module) {
    return module->state->value;
}
