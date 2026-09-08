#include "types.h"

typedef struct MovieModuleState {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
} MovieModuleState;

void fn_12_2D450(MovieModuleState *state) {
    state->field_0 = 0;
    state->field_4 = 0;
    state->field_8 = -3;
    state->field_c = 1;
}
