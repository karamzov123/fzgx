#include "types.h"

typedef struct MovieModuleState {
    u8 pad_0[0x78];
    s32 field_78;
    u8 pad_7c[0x8c];
    s32 field_108;
    u8 field_10c[1];
} MovieModuleState;

extern s32 fn_12_24990(MovieModuleState *state);
extern s32 fn_12_24A88(s32 arg0, u32 arg1);

s32 fn_12_2D470(MovieModuleState *state, void **out0, u32 *out1) {
    if (fn_12_24990(state)) {
        return fn_12_24A88(0, 0xff00011c);
    }
    if (state->field_78) {
        *out0 = &state->field_10c[0];
        *out1 = state->field_108;
    } else {
        *out0 = 0;
        *out1 = 0;
    }
    return 0;
}
