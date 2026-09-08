#include "types.h"

struct MovieState {
    char pad_0[4];
    int value;
};

struct MovieModule {
    char pad_0[0x94c];
    struct MovieState state;
};

typedef void (*MovieCallback)(struct MovieModule *, void *, struct MovieState *);

extern MovieCallback fn_12_2D73C(struct MovieModule *, int);

void fn_12_2B9F8(struct MovieModule *module, int delta, void *context) {
    struct MovieState *state;
    MovieCallback callback;

    state = &module->state;
    module->state.value += delta;
    callback = fn_12_2D73C(module, 0x25);
    if (callback != 0) {
        callback(module, context, state);
    }
}
