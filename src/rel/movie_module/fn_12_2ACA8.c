#include "types.h"

typedef struct MovieModule MovieModule;

extern int fn_12_24990(MovieModule *module, const char *name);
extern int fn_12_24A88(int value, u32 address);
extern void fn_12_2D7DC(MovieModule *module, int value, const char *name);

int fn_12_2ACA8(MovieModule *module, const char *name) {
    if (fn_12_24990(module, name)) {
        return fn_12_24A88(0, 0xff000146);
    }

    fn_12_2D7DC(module, 0x1d, name);
    return 0;
}
