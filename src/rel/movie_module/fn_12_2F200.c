#include "types.h"

struct MovieItem {
    int value;
    char pad[0x40];
};

struct Movie {
    char pad[0x1aa0];
    struct MovieItem items[1];
};

void fn_12_2F200(struct Movie *movie, int index, int value) {
    movie->items[index].value = value;
}
