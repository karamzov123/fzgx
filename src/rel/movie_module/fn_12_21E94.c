#include "types.h"

typedef struct MovieObject MovieObject;
typedef int (*MovieCallback)(MovieObject *, int);

typedef struct MovieVtable {
    char padding[0x24];
    MovieCallback callback;
} MovieVtable;

struct MovieObject {
    MovieVtable *vtable;
};

typedef struct MovieModule {
    char padding0[0x1154];
    MovieObject *object;
    char padding1[0x18];
    int value;
    int delta;
} MovieModule;

extern void fn_12_24970(void *movie);
extern void fn_12_24950(void *movie);

int fn_12_21E94(MovieModule *module, int index) {
    char movie[8];
    MovieModule *entry;
    int value;
    int delta;

    entry = (MovieModule *)((char *)module + index * 0x74);
    fn_12_24970(movie);
    value = entry->value;
    delta = entry->delta;
    if (value == 0 && delta != 0) {
        value = delta + entry->object->vtable->callback(entry->object, 1);
    }
    if (value < 0) {
        value = 0x7fffffff;
    }
    fn_12_24950(movie);
    return value;
}
