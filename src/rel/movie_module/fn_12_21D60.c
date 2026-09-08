#include "types.h"

typedef struct MovieEntry {
    int value;
    u8 padding[0x70];
} MovieEntry;

typedef struct MovieModule {
    u8 padding[0x1148];
    MovieEntry entries[1];
} MovieModule;

void fn_12_21D60(MovieModule *module, int index, int value) {
    module->entries[index].value = value;
}
