#include "types.h"

extern void fn_12_21AE4(void *);

typedef struct MovieModule {
    u8 pad_00[0x40];
    void *field_40;
} MovieModule;

void fn_12_3412C(MovieModule *self) {
    fn_12_21AE4(self->field_40);
}
