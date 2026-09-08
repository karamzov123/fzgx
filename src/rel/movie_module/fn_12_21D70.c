#include "types.h"

typedef struct MovieRecord {
    u8 padding00[0x1144];
    int status;
    u8 padding1148[0x44];
    void *resource;
} MovieRecord;

typedef struct MovieModule {
    u8 padding00[0x44];
    int active;
} MovieModule;

extern int fn_12_2F210(MovieModule *module, void *resource, int size, void *argument, int flags);

int fn_12_21D70(MovieModule *module, int index, void *argument) {
    int result = 0;
    MovieRecord *entry = (MovieRecord *)((u8 *)module + index * 0x74);

    if (entry->status == 0) {
        result = fn_12_2F210(module, entry->resource, 0xc, argument, 0);
    }

    module->active = 1;
    return result;
}
