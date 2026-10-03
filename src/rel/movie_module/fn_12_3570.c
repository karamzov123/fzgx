#include "types.h"

typedef struct MovieModuleObject {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
} MovieModuleObject;

extern u32 lbl_12_bss_4C0[42];

static inline MovieModuleObject *find_object(void) {
    MovieModuleObject *cur = (MovieModuleObject *)(lbl_12_bss_4C0 + 2);
    int i;
    for (i = 0; i < (int)lbl_12_bss_4C0[1]; i++) {
        if (cur->field_0 == 0) {
            return cur;
        }
        cur++;
    }
    return 0;
}

#pragma opt_dead_assignments off
MovieModuleObject *fn_12_3570(void) {
    int fzgx_value;
    MovieModuleObject *obj = find_object();
    if (obj != 0) {
        obj->field_8 = 0;
        fzgx_value = 0x1f;
        obj->field_C = fzgx_value;
        obj->field_10 = 0x64;
        obj->field_4 = 1;
        lbl_12_bss_4C0[0] += 1;
        obj->field_0 = 1;
    }
    return obj;
}
#pragma opt_dead_assignments reset

