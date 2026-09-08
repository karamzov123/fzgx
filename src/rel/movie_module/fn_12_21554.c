#include "types.h"

typedef struct MovieInner {
    void *value;
} MovieInner;

typedef struct Movie {
    u8 pad_0000[0x1b74];
    MovieInner *inner;
} Movie;

extern void fn_8004BED0(void *value);

void fn_12_21554(Movie *movie) {
    fn_8004BED0(movie->inner->value);
}
