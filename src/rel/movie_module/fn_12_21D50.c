#include "types.h"

typedef struct MovieEntry {
    int value;
    char unk04[0x70];
} MovieEntry;

typedef struct MovieModule {
    char unk0000[0x1148];
    MovieEntry entries[1];
} MovieModule;

int fn_12_21D50(MovieModule *module, int index) {
    return module->entries[index].value;
}
