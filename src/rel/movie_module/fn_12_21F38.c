#include "types.h"

typedef struct MovieEntry {
    int value;
    u8 pad[0x70];
} MovieEntry;

typedef struct MovieModule {
    u8 pad[0x115c];
    MovieEntry entries[1];
} MovieModule;

int fn_12_21F38(MovieModule *module, int index) {
    return module->entries[index].value;
}
