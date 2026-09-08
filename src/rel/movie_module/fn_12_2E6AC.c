#include "types.h"

typedef struct MovieModuleState {
    u8 padding[0x48];
    s32 state;
} MovieModuleState;

s32 fn_12_2E6AC(MovieModuleState *movie, s32 *result, s32 *flag) {
    s32 valid;
    s32 state = movie->state;

    if (state != 4 && state != -4 && state != 6 && state != -6) {
        *result = -1;
        valid = 0;
        *flag = 1;
    } else {
        valid = 1;
    }

    if (valid == 0) {
        return 0;
    }

    *result = -2;
    valid = 0;
    *flag = 1;
    return valid;
}
