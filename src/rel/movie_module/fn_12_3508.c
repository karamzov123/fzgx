#include "types.h"

typedef struct MovieModuleObject {
    int field_0;
    int field_4;
    void *field_8;
    void *field_C;
    void *field_10;
    int field_14;
} MovieModuleObject;

extern void fn_12_3BEF0(void *, void *, void *, void *);

void fn_12_3508(MovieModuleObject *module, void *arg1, void *arg2) {
    fn_12_3BEF0(module->field_8, module->field_C, module->field_10, arg2);
    module->field_4 = 0;
}
