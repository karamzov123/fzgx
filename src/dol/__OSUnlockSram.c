#include "types.h"

extern u32 UnlockSram(u32, u32);

void __OSUnlockSram(u32 arg0) {
    UnlockSram(arg0, 0);
}
