#include "types.h"

extern int fn_12_24990(void *arg0, void *arg1);
extern int fn_12_24A88(int arg0, u32 arg1);

typedef struct MovieQueueEntry {
    void *data;
    int unk;
} MovieQueueEntry;

typedef struct MovieQueue {
    void *source;
    int count;
    int stride;
    void *base;
    int unk10;
    MovieQueueEntry entries[16];
} MovieQueue;

typedef struct MovieState {
    u8 pad[0x8b8];
    MovieQueue queue;
} MovieState;

typedef struct MovieModule {
    u8 pad[0x1b30];
    MovieState *state;
} MovieModule;

int fn_12_2A434(MovieModule *module, void *source, int count, int stride) {
    MovieState *state;
    MovieQueue *queue;
    int i;

    if (fn_12_24990(module, source)) {
        return fn_12_24A88(0, 0xff000185);
    }

    state = module->state;
    queue = &state->queue;
    queue->source = source;
    queue->count = count < 0x10 ? count : 0x10;
    queue->stride = stride;
    queue->base = source;
    queue->unk10 = 0;
    for (i = 0; i < queue->count; i++) {
        queue->entries[i].data = (u8 *)source + stride * (i + 1);
        queue->entries[i].unk = 0;
    }
    return 0;
}

