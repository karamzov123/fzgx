#include "types.h"

typedef struct MovieEntry {
    int used;
    u8 pad[0x74];
} MovieEntry;

typedef struct MovieList {
    u8 pad[0x134];
    int count;
    MovieEntry entries[1];
} MovieList;

typedef struct MovieObject {
    u8 pad[0x1b30];
    MovieList *list;
} MovieObject;

struct MovieContext {
    u32 words[2];
};

extern void fn_12_24970(struct MovieContext *context);
extern void fn_12_24950(struct MovieContext *context);

MovieEntry *fn_12_2AB68(MovieObject *movie) {
    struct MovieContext context;
    int i;
    MovieList *list;
    MovieEntry *entry;
    int count;

    fn_12_24970(&context);
    list = movie->list;
    count = list->count;
    entry = list->entries;
    i = 0;
    while (i < count) {
        if (entry->used == 0) {
            entry->used = 1;
            break;
        }
        i++;
        entry++;
    }
    if (i == count) {
        entry = 0;
    }
    fn_12_24950(&context);
    return entry;
}
