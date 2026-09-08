#include "types.h"

struct MovieModule {
    u8 pad_0x1c48[0x1c48];
    void *field_0x1c48;
};

extern void fn_12_21D70(struct MovieModule *, void *, void *);

void fn_12_2FB5C(struct MovieModule *module, void *arg) {
    fn_12_21D70(module, module->field_0x1c48, arg);
}
