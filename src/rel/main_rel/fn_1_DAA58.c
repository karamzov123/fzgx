#include "types.h"

typedef struct {
    u8 pad_0000[0xbd4];
    u32 flag;
    u8 state[1];
} State;

extern State *lbl_1_data_2A7E0[62];
extern void fn_1_FB9C0(u32 value);
extern void fn_1_103F10(void *state);
extern void fn_1_DAB3C(State *state);

void fn_1_DAA58(void) {
    State *state = lbl_1_data_2A7E0[15];

    if (state->flag != 0) {
        fn_1_FB9C0(0);
        fn_1_FB9C0(1);
        fn_1_FB9C0(2);
        fn_1_FB9C0(4);
    }
    fn_1_103F10(&state->state[0]);
    fn_1_DAB3C(state);
}
