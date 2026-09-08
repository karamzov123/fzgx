#include "types.h"

typedef struct MovieModule {
    u8 pad[0xa8];
    void *field_a8;
} MovieModule;

extern void fn_12_49EC(void *);

void fn_12_35060(MovieModule *self) {
    fn_12_49EC(self->field_a8);
}
