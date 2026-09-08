#include "types.h"

typedef struct MovieModule {
    u8 pad_0[0xf28];
    int value;
} MovieModule;

int fn_12_2DAB4(MovieModule *module) {
    return module->value;
}
