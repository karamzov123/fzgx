#include "types.h"

typedef struct MovieModule {
    u8 pad_00[0x40];
    void *field_40;
} MovieModule;

extern void fn_12_21A64(void *);

void fn_12_34198(MovieModule *module) {
    fn_12_21A64(module->field_40);
}
