#include "types.h"

typedef struct MovieModuleState {
    int field_0;
    int field_4;
    int field_8;
} MovieModuleState;

void fn_12_2F1A0(MovieModuleState *state, int value) {
    state->field_0 = 0;
    state->field_4 = 0;
    state->field_8 = value;
}
