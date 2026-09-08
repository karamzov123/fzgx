#include "types.h"

typedef struct Fn80025C28State {
    u8 _pad00[0x30];
    u32 field30;
    u32 field34;
    u32 field38;
} Fn80025C28State;

extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32);
extern void fn_80025A24(Fn80025C28State *);

void fn_80025C28(Fn80025C28State *state) {
    u32 interrupts;

    interrupts = OSDisableInterrupts();
    state->field30 = 0;
    state->field34 = 0;
    state->field38 = 0;
    OSRestoreInterrupts(interrupts);
    fn_80025A24(state);
}
