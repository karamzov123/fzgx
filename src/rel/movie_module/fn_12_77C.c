#include "types.h"

typedef struct MovieModule {
    u8 pad2c[0x2c];
    void *value;
} MovieModule;

extern void fn_12_34C4(void *value);

void fn_12_77C(MovieModule *self) {
    fn_12_34C4(self->value);
}
