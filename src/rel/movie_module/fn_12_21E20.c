#include "types.h"

typedef struct MovieEntry {
    char unk00[0x2c];
    int value;
    char unk30[0x44];
} MovieEntry;

typedef struct MovieModule {
    char unk0000[0x1148];
    MovieEntry entries[1];
} MovieModule;

void fn_12_21E20(MovieModule *module, int index, int delta) {
    if (module->entries[index].value < 0) {
        return;
    }
    module->entries[index].value += delta;
}
