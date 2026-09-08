#include "types.h"

struct MovieModule {
    char pad_960[0x960];
    int value_960;
};

extern int fn_12_24990(struct MovieModule *module);
extern void fn_12_24A88(int value, u32 code);
extern void fn_12_2F210(struct MovieModule *module, int value, int size, void *data, int flags);

void fn_12_2B25C(struct MovieModule *module, void *data) {
    *(u32 *)data = 0;
    if (fn_12_24990(module)) {
        fn_12_24A88(0, 0xff000136);
    } else {
        fn_12_2F210(module, 6, 11, data, 0);
        if (*(u32 *)data != 0) {
            module->value_960 = 1;
        }
    }
}
