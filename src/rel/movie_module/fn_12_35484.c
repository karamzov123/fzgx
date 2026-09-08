#include "types.h"

typedef struct Movie Movie;
typedef void (*MovieCallback)(Movie *);

typedef struct MovieVTable {
    u8 pad_0c[0x0c];
    MovieCallback callback;
} MovieVTable;

struct Movie {
    MovieVTable *vtable;
};

typedef struct MovieModuleState {
    u8 pad_e8[0xe8];
    Movie *movie;
} MovieModuleState;

void fn_12_35484(MovieModuleState *self) {
    Movie *movie = self->movie;

    if (movie != 0) {
        movie->vtable->callback(movie);
    }
}
