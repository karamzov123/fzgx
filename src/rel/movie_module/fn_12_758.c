#include "types.h"

extern void fn_12_34A8(void *);

typedef struct MovieModule {
    u8 pad2c[0x2c];
    void *value;
} MovieModule;

void fn_12_758(MovieModule *self) {
    fn_12_34A8(self->value);
}
