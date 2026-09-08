#include "types.h"

typedef struct MovieModuleState {
    u8 padding_000[0x10c];
    u32 field_10c;
    u32 field_110;
} MovieModuleState;

void fn_12_2E9E4(MovieModuleState *state, u32 value1, u32 value2) {
    state->field_10c = value1;
    state->field_110 = value2;
}
