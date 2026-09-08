#include "types.h"

typedef struct Movie Movie;
typedef struct MovieVTable MovieVTable;

struct MovieVTable {
    u8 pad_00[0x14];
    void (*method_14)(Movie *self);
};

struct Movie {
    MovieVTable *vtable;
};

typedef struct MovieModuleState {
    u8 pad_e8[0xe8];
    Movie *movie;
} MovieModuleState;

void fn_12_353C4(MovieModuleState *self) {
    Movie *movie = self->movie;

    if (movie != 0) {
        movie->vtable->method_14(movie);
    }
}
