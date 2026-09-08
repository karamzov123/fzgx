#include "types.h"

typedef struct MovieModuleState {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
} MovieModuleState;

void fn_12_2EE48(MovieModuleState *state, int value) {
    state->field_0 = 0;
    state->field_4 = 0;
    state->field_8 = 0;
    state->field_C = 0;
    state->field_10 = 0;
    state->field_14 = 0;
    state->field_18 = 0;
    state->field_1C = 0;
    state->field_20 = value;
    state->field_24 = 1;
}
