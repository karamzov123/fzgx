#include "types.h"

typedef struct MovieModuleState {
    u8 pad_20[0x20];
    s32 state;
} MovieModuleState;

u32 fn_12_35548(MovieModuleState *self) {
    if (self->state == 0 || self->state == 0x101) {
        return 1;
    }
    return 0;
}
