#include "types.h"

typedef struct {
    u32 flags;
    u8 pad[0x46];
    u8 status;
} State;

extern State *lbl_1_bss_F68;
extern void fn_1_6D2C(u32);

void fn_1_6CD8(void) {
    State *state = lbl_1_bss_F68;
    u32 value;

    if (state == 0) {
        value = 0;
    } else if ((state->flags & ((u32)1 << 31)) != 0) {
        value = 0;
    } else {
        value = state->status;
    }

    fn_1_6D2C(value);
}
