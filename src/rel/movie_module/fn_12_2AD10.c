#include "types.h"

extern int fn_12_24990(void *module);
extern int fn_12_24A88(int value, int code);
extern void fn_12_2D7DC(void *module, int value, int argument);

int fn_12_2AD10(void *module, int argument) {
    if (fn_12_24990(module)) {
        return fn_12_24A88(0, 0xff000145);
    }
    fn_12_2D7DC(module, 0x1e, argument);
    return 0;
}
