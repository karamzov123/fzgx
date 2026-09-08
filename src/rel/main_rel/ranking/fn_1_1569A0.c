#include "types.h"

extern void fn_8006B7B4(void *);

typedef struct {
    u32 flags;
    u8 _pad04[4];
    void *data;
} State;

void fn_1_1569A0(State *state) {
    if (state->flags & 1) {
        fn_8006B7B4(state->data);
        state->flags &= ~1;
    }
}
