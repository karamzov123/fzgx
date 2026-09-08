#include "types.h"

typedef struct MovieModuleState {
    u8 _pad_0[0x44];
    u32 field_44;
    u32 field_48;
} MovieModuleState;

u32 fn_12_2CD44(MovieModuleState *state) {
    if (state->field_48 - 1 > 3) {
        return 1;
    }
    return state->field_44 == 0;
}
