#include "types.h"

typedef struct MovieModule {
    u8 pad[0xa8];
    void *field_a8;
} MovieModule;

extern void fn_12_7A0(void *arg);

void fn_12_35138(MovieModule *self) {
    fn_12_7A0(self->field_a8);
}
