#include "types.h"

extern void fn_12_219E8(void *);

typedef struct MovieModule {
    u8 pad_00[0x40];
    void *field_40;
} MovieModule;

void fn_12_34174(MovieModule *self) {
    fn_12_219E8(self->field_40);
}
