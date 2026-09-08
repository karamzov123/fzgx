#include "types.h"

struct MovieModule {
    char pad_960[0x960];
    int value_960;
};

extern int fn_12_24990(void);
extern void fn_12_24A88(int value, u32 code);
extern void fn_12_2F210(struct MovieModule *module, int value, int size, void *data, int flags);

void fn_12_2B1E8(struct MovieModule *module, void *data) {
    if (fn_12_24990()) {
        fn_12_24A88(0, 0xff000137);
    } else {
        module->value_960 = 0;
        fn_12_2F210(module, 6, 12, data, 0);
    }
}

