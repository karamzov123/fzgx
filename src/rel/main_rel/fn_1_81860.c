#include "types.h"

typedef struct {
    u8 pad0[2];
    s16 id;
    u8 pad4[8];
    u32 flags;
} Obj;

s32 fn_1_81860(Obj *a, Obj *b) {
    u32 flags_a = a->flags & 0xF0000000;
    if (flags_a != 0 && (b->flags & 0xF0000000) == 0) {
        return 1;
    }
    if ((b->flags & 0xF0000000) != 0 && flags_a == 0) {
        return -1;
    }
    return a->id - b->id;
}
