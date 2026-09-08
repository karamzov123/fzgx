#include "types.h"

typedef struct MovieModuleState {
    u8 pad_a8[0xa8];
    void *value_a8;
} MovieModuleState;

extern void fn_12_8DC(void *);

void fn_12_356E8(MovieModuleState *self) {
    fn_12_8DC(self->value_a8);
}
