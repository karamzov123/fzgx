#include "types.h"

typedef struct MovieObject MovieObject;

typedef struct MovieVtable {
    u32 pad[3];
    void (*fn0c)(MovieObject *);
} MovieVtable;

struct MovieObject {
    MovieVtable *vtable;
};

typedef struct MovieSub {
    u32 pad;
    MovieObject *obj;
} MovieSub;

typedef struct MovieEntry {
    int field;
    u8 pad[0xC];
    MovieSub sub;
    u8 pad2[0x5C];
} MovieEntry;

void fn_12_22888(u8 *module) {
    MovieEntry *entries = (MovieEntry *)(module + 0x1140);
    MovieSub *s;
    MovieObject *o;

    s = &entries[0].sub;
    if (entries[0].field == 5 && (o = s->obj) != 0) {
        o->vtable->fn0c(o);
        s->obj = 0;
    }
    s = &entries[1].sub;
    if (entries[1].field == 5 && (o = s->obj) != 0) {
        o->vtable->fn0c(o);
        s->obj = 0;
    }
    s = &entries[2].sub;
    if (entries[2].field == 5 && (o = s->obj) != 0) {
        o->vtable->fn0c(o);
        s->obj = 0;
    }
}
