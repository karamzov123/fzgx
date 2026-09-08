#include "types.h"

extern int fn_12_2D73C(void *self, int arg);

typedef struct MovieModule {
    u8 pad_0000[0x1c84];
    void *field_1c84;
} MovieModule;

int fn_12_218D8(MovieModule *self) {
    if (fn_12_2D73C(self, 6) == 0) {
        return 0;
    }
    self->field_1c84 = (u8 *)self + 0x28b8;
    return 0;
}
