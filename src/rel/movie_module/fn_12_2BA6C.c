#include "types.h"

struct MovieModule {
    char pad_94c[0x94c];
    int value_94c;
};

typedef void (*MovieCallback)(struct MovieModule *, void *, int *);

extern MovieCallback fn_12_2D73C(struct MovieModule *, int);

void fn_12_2BA6C(struct MovieModule *module, int delta, void *arg) {
    int *value = &module->value_94c;
    MovieCallback callback;

    *value += delta;
    callback = fn_12_2D73C(module, 0x24);
    if (callback != 0) {
        callback(module, arg, value);
    }
}
