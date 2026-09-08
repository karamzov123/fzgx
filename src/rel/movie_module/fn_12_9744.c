#include "types.h"

typedef struct MovieState {
    u8 _pad10[0x10];
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1c;
} MovieState;

void fn_12_9744(MovieState *movie) {
    movie->field_10 = 0;
    movie->field_14 = 0;
    movie->field_18 = 0;
    movie->field_1c = 0;
}
