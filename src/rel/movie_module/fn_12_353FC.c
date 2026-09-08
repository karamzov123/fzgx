#include "types.h"

typedef struct MovieModuleState {
    u8 pad_fc[0xfc];
    s32 status;
    s32 value_100;
    s32 value_104;
    s32 value_108;
} MovieModuleState;

void fn_12_353FC(MovieModuleState *self) {
    self->value_100 = 0;
    self->value_104 = 0;
    self->value_108 = 0;
    self->status = -1;
}
