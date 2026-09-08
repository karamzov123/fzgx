#include "types.h"

typedef struct MovieState {
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_c;
    u32 field_10;
} MovieState;

void fn_12_A774(MovieState *movie) {
    movie->field_0 = 0;
    movie->field_4 = 0;
    movie->field_8 = 0;
    movie->field_c = 0;
    movie->field_10 = 0;
}
