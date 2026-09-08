#include "types.h"

typedef struct MovieModuleObject {
    u32 field_0;
} MovieModuleObject;

extern u32 lbl_12_bss_4C0[42];

void fn_12_354C(MovieModuleObject *module) {
    if (module != 0) {
        module->field_0 = 0;
        lbl_12_bss_4C0[0] -= 1;
    }
}
