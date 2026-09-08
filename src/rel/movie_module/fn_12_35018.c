#include "types.h"

typedef struct MovieModule {
    u8 pad_a8[0xa8];
    void *field_a8;
} MovieModule;

extern void fn_12_49AC(void *);

void fn_12_35018(MovieModule *self) {
    fn_12_49AC(self->field_a8);
}
