#include "types.h"

extern void fn_12_24970(int *arg0);
extern void fn_12_24950(int *arg0);

typedef struct MovieModuleState {
    u8 pad_000[0x120];
    int field_120;
    int field_124;
    int field_128[32];
} MovieModuleState;

void fn_12_23120(void *arg0, int arg1) {
    MovieModuleState *state = (MovieModuleState *)((u8 *)arg0 + 0xcc0);
    int local;
    int next;
    int total;

    fn_12_24970(&local);
    state->field_120 += arg1;
    next = state->field_124;
    next = next + 1;
    total = state->field_120;
    state->field_128[next % 32] = total;
    state->field_124 = next;
    fn_12_24950(&local);
}
