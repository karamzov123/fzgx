#include "types.h"

typedef struct MovieEntry {
    u8 pad[0x1168];
    int value1;
    int value2;
} MovieEntry;

typedef struct MovieModule {
    u8 pad[1];
} MovieModule;

extern void fn_12_24970(void *);
extern void fn_12_24950(void *);

void fn_12_21FA4(MovieModule *module, int index, int *value1, int *value2) {
    MovieEntry *entry = (MovieEntry *)((u8 *)module + index * 0x74);
    u8 movie_data[8];

    fn_12_24970(movie_data);
    *value1 = entry->value1;
    *value2 = entry->value2;
    fn_12_24950(movie_data);
}
