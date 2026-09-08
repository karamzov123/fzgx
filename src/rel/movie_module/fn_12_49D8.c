#include "types.h"

typedef struct MovieState {
    u8 pad[0x58];
    s32 value;
} MovieState;

int fn_12_49D8(MovieState *state) {
    return state->value != 0;
}
