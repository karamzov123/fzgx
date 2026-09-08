#include "types.h"

struct MovieEntry {
    int value;
    char padding[0x70];
};

struct MovieModule {
    char padding[0x1174];
    struct MovieEntry entries[1];
};

int fn_12_21F28(struct MovieModule *module, int index) {
    return module->entries[index].value;
}
