#include "types.h"

extern void fn_12_24970(void *);
extern void fn_12_24950(void *);

typedef struct MovieState {
    u8 unk_000[0x1a8];
    int pending;
    u8 unk_1ac[4];
    s32 head;
    s32 tail;
    void *entries[0x20];
} MovieState;

typedef struct MovieModule {
    u8 unk_000[0xcc0];
    MovieState state;
} MovieModule;

int fn_12_23078(MovieModule *module, void *entry, int value) {
    u8 local[8];
    MovieState *state = &module->state;
    int pending = value;

    fn_12_24970(local);
    if (state->head - state->tail >= 0x20) {
        pending = 0;
    } else {
        state->pending = pending;
        state->entries[state->head % 0x20] = entry;
        pending = 1;
        state->head++;
    }
    fn_12_24950(local);
    return pending;
}
