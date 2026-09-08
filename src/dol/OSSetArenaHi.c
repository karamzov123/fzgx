#include "types.h"

extern u32 __OSArenaHi_801A6790;

void OSSetArenaHi(u32 arg0) {
    __OSArenaHi_801A6790 = arg0;
}
