#include "types.h"

typedef struct {
    s32 flags;
    u8 pad[0x44];
    s8 mode;
    u8 pad49;
    u8 status;
} State;

typedef struct {
    u8 pad[6];
    s16 value;
} Result;

extern State *lbl_1_bss_F68;
extern Result *lbl_1_bss_F70;

s16 fn_1_6AE4(void) {
    State *state = lbl_1_bss_F68;
    u32 value;

    if (state->mode == 6) {
        if (state == 0) {
            value = 0;
        } else if ((state->flags & ((s32)1 << 31)) != 0) {
            value = 0;
        } else {
            value = state->status;
        }

        if (value == 3) {
            return lbl_1_bss_F70->value;
        }
    }

    return -1;
}
