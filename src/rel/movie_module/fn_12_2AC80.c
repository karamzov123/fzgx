#include "types.h"

typedef struct MovieObject MovieObject;
typedef struct MovieState MovieState;

struct MovieObject {
    u8 _pad_1b30[0x1b30];
    MovieState *state;
};

struct MovieState {
    u8 _pad_80[0x80];
    int value;
};

void fn_12_2AC80(MovieObject *movie, int value) {
    movie->state->value = value;
}
