#include "types.h"

extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32 level);

typedef struct Fn80023168State {
    u8 pad_000[0x1c];
    u32 flags;
    u32 state;
    u8 pad_024[0x122];
    u16 value;
} Fn80023168State;

void fn_80023168(Fn80023168State *state, u16 value) {
    u32 level = OSDisableInterrupts();

    state->value = value;
    state->flags |= 4;
    if (value == 0) {
        state->state = 1;
    }

    OSRestoreInterrupts(level);
}
