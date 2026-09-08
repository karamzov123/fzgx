#include "types.h"

struct MovieSlot {
    int value;
    char pad[0x40];
};

struct Movie {
    char pad[0x1aa4];
    struct MovieSlot slots[1];
};

void fn_12_2F1E0(struct Movie *movie, int index, int value) {
    movie->slots[index].value = value;
}
