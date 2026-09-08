#include "types.h"

typedef struct MovieModule {
    u8 pad_28[0x28];
    void *field_28;
} MovieModule;

extern void fn_12_4F3C(MovieModule *module, void *arg);

void fn_12_4F18(MovieModule *module) {
    fn_12_4F3C(module, module->field_28);
}
