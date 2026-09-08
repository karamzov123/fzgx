#include "types.h"

typedef struct {
    u32 flags;
    u8 pad[0x46];
    u8 status;
} State;

extern State *lbl_1_bss_F68;
extern u32 fn_1_6514(u32);

u32 fn_1_64C0(void) {
    State *state;
    u32 value;

    state = lbl_1_bss_F68;
    if (state == 0) {
        value = 0;
    } else if ((state->flags & ((u32)1 << 31)) != 0) {
        value = 0;
    } else {
        value = state->status;
    }

    return fn_1_6514(value);
}
