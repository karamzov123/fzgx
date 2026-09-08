#include "types.h"

typedef struct MovieState {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
} MovieState;

void fn_12_34A8(MovieState *self, u32 *value_c, u32 *value_10, u32 *value_8) {
    *value_8 = self->unk8;
    *value_c = self->unkC;
    *value_10 = self->unk10;
}
