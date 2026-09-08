#include "types.h"

typedef struct MovieData {
    u8 _pad0[0x3c];
    f32 value0;
    f32 value1;
} MovieData;

void fn_12_365C(const MovieData *movie, f32 *out0, f32 *out1) {
    *out0 = movie->value0;
    *out1 = movie->value1;
}
