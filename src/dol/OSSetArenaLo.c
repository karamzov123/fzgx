#include "types.h"

extern u32 __OSArenaLo_801A6428;

void OSSetArenaLo(u32 arg0) {
    __OSArenaLo_801A6428 = arg0;
}
