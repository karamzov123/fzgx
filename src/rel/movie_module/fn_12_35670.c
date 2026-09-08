#include "types.h"

typedef struct MovieModuleState {
    u8 pad_a8[0xa8];
    void *movie;
} MovieModuleState;

extern void fn_12_750(void *movie);

void fn_12_35670(MovieModuleState *self) {
    fn_12_750(self->movie);
}
