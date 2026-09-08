#include "types.h"

typedef struct MovieState {
    u32 unk0;
    u32 active;
    u32 unk8;
    u32 value4;
    u32 value5;
} MovieState;

void fn_12_34C4(MovieState *self, u32 value4, u32 value5, u32 value8) {
    self->unk8 = value8;
    self->value4 = value4;
    self->value5 = value5;
    self->active = 1;
}
