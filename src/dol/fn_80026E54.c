#include "types.h"

typedef struct {
    u32 unk0;
    volatile u32 flags; /* Hardware-visible flag state must be written in two steps. */
    u8 padding[0x58];
} FlagEntry;

typedef struct {
    u8 padding[0x18];
    u32 index;
} Fn80026E54Arg;

extern FlagEntry lbl_80176160[32];

void fn_80026E54(Fn80026E54Arg *arg) {
    FlagEntry *entry = &lbl_80176160[arg->index];

    entry->flags &= 0xFFFFFFFD;
    entry->flags |= 0x40000000;
}

