#include "types.h"

typedef struct MovieModuleState {
    u8 padding_000[0x250];
    int field_250;
    int field_254;
    int field_258;
    int field_25C;
} MovieModuleState;

void fn_12_2EDAC(MovieModuleState *state, int value) {
    int difference;
    int result;
    int current;

    if (state->field_250 == -5) {
        state->field_250 = value;
        return;
    }

    difference = value - state->field_250;
    if (difference == 0) {
        return;
    }

    state->field_250 = value;

    result = difference;
    current = state->field_258;
    if (current > difference) {
        result = current;
    }
    state->field_258 = result;

    result = difference;
    current = state->field_25C;
    if (current < difference) {
        result = current;
    }
    state->field_25C = result;

    current = state->field_254;
    if (current == 0x7fffffff) {
        state->field_254 = difference;
        return;
    }

    if (current <= difference) {
        state->field_254 = difference;
        return;
    }

    result = (current - difference) / 8;
    if (result != 0) {
        state->field_254 = current - result;
        return;
    }

    state->field_254 = difference;
}
