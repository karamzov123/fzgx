#include "types.h"

struct MovieData {
    u8 pad[0x64];
    int value;
};

int fn_12_738(struct MovieData *movie) {
    return movie->value;
}
