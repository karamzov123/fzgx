#include "types.h"

typedef struct {
    u8 pad[5];
    s8 value;
} Fn800542BC;

int fn_800542BC(Fn800542BC *obj) {
    return obj->value;
}
