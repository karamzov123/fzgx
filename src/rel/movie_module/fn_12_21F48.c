#include "types.h"

typedef struct MovieObject {
    u8 pad[0x1168];
    void *movie;
    void *stream;
} MovieObject;

typedef struct MovieState {
    u8 data[4];
} MovieState;

extern void fn_12_24970(MovieState *state);
extern void fn_12_24950(MovieState *state);

void fn_12_21F48(u8 *module, int index, void *movie, void *stream) {
    MovieState state;
    MovieObject *entry;

    entry = (MovieObject *)(module + index * 0x74);
    fn_12_24970(&state);
    entry->movie = movie;
    entry->stream = stream;
    fn_12_24950(&state);
}
