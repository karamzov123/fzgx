#include "types.h"

typedef struct MovieModule {
    u8 pad30[0x30];
    int value;
} MovieModule;

int fn_12_748(MovieModule *self) {
    return self->value;
}
