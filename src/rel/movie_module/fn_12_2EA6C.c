#include "types.h"

typedef struct MovieModuleState {
    u8 padding_000[0x118];
    int field_118;
} MovieModuleState;

extern int fn_12_334A0(int value1, int value2, int value3);
extern u32 lbl_12_bss_1B73C;
extern u32 lbl_12_bss_1B738;

int fn_12_2EA6C(MovieModuleState *state, int value) {
    int result;

    if (state->field_118 < 0) {
        return -1;
    }

    result = fn_12_334A0(state->field_118, value, 0x57e4);
    lbl_12_bss_1B73C = state->field_118;
    lbl_12_bss_1B738 = result;
    return result;
}
