#include "types.h"

struct Movie;

typedef int (*MovieHandler)(struct Movie *, int, int, int);

struct MovieItem {
    char pad0[0xc];
    MovieHandler *handlers;
    char pad1[0x30];
    char pad2[4];
};

struct Movie {
    char pad0[0x1aa0];
    struct MovieItem items[1];
};

int fn_12_2F264(struct Movie *movie, int index) {
    int i;
    int result;
    struct MovieItem *item;

    result = 0;
    item = movie->items;
    for (i = 0; i < 9; i++, item++) {
        if (item->handlers != 0) {
            result = item->handlers[index](movie, 0, 0, 0);
            if (result != 0) {
                break;
            }
        }
    }
    return result;
}
