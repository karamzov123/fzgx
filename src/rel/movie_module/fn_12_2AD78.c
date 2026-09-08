#include "types.h"

struct MovieModule {
    char pad_4c[0x4c];
    int value;
};

int fn_12_2AD78(struct MovieModule *module) {
    module->value = 3;
    return 0;
}
