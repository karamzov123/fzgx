#include "types.h"

typedef struct MovieModuleState {
    u8 _pad_0[0x44];
    u32 field_44;
} MovieModuleState;

extern int fn_12_24990(MovieModuleState *state);
extern int fn_12_24A88(int arg0, u32 arg1);
extern void fn_12_2C9C4(MovieModuleState *state);

int fn_12_2CC24(MovieModuleState *state) {
    if (fn_12_24990(state)) {
        return fn_12_24A88(0, 0xFF000138);
    }
    state->field_44 = 1;
    fn_12_2C9C4(state);
    return 0;
}
