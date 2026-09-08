#include "types.h"

typedef struct MovieObject {
    u8 pad[0x60];
    s32 value;
} MovieObject;

void fn_12_3A968(MovieObject *self, s32 value) {
    self->value = value;
}
