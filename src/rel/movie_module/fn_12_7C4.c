#include "types.h"

typedef struct MovieModule {
    u8 pad20[0x20];
    void *value;
} MovieModule;

extern void fn_12_3670(void *value);

void fn_12_7C4(MovieModule *self) {
    fn_12_3670(self->value);
}
