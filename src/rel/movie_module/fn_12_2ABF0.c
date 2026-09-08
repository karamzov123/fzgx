#include "types.h"

typedef struct MovieObject MovieObject;
typedef struct MovieState MovieState;
typedef struct MovieEntry MovieEntry;

extern void fn_12_24970(void *value);
extern void fn_12_24950(void *value);

struct MovieObject {
    u8 _pad_1b30[0x1b30];
    MovieState *state;
};

struct MovieEntry {
    int type;
    u8 _pad_74[0x74];
};

struct MovieState {
    u8 _pad_7c[0x7c];
    int status;
    u8 _pad_134[0xb4];
    int count;
    u8 entries[1];
};

int fn_12_2ABF0(MovieObject *movie) {
    MovieState *state;
    MovieEntry *entry;
    int count;
    int result;
    u8 value[8];

    fn_12_24970(value);
    state = movie->state;
    result = 0;
    count = state->count;
    entry = (MovieEntry *)state->entries;
    while (count > 0) {
        if (entry->type == 2 || entry->type == 4) {
            result++;
        }
        entry++;
        count--;
    }
    if (state->status == 1 && result == 0) {
        result = -1;
    }
    fn_12_24950(value);
    return result;
}
