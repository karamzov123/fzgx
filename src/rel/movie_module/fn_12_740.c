#include "types.h"

typedef struct MovieModule {
    u8 pad[0x60];
    int value;
} MovieModule;

int fn_12_740(MovieModule *self) {
    return self->value;
}
