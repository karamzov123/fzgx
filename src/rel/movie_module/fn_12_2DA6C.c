#include "types.h"

typedef struct MovieModule {
    u8 pad_0[0x48];
    u32 value;
} MovieModule;

extern int fn_12_24990(MovieModule *module);
extern void fn_12_24A88(int arg0, int arg1);

u32 fn_12_2DA6C(MovieModule *module) {
    if (fn_12_24990(module) != 0) {
        fn_12_24A88(0, (int)0xff000111);
    }
    return module->value;
}
