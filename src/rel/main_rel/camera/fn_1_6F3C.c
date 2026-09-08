#include "types.h"

typedef struct {
    u32 flags;
    u8 pad[0x44];
    s8 status;
} State;

extern State *lbl_1_bss_F68;

u32 fn_1_6F3C(void) {
    State *state = lbl_1_bss_F68;

    if (state == 0) {
        return 0;
    }
    if ((state->flags & ((u32)1 << 31)) != 0) {
        return 0;
    }
    return state->status == 0x0B;
}
