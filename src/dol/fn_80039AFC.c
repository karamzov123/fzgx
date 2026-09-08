#include "types.h"

extern u32 __cpReg;

void fn_80039AFC(void) {
    /* Volatile is required for the memory-mapped GX register. */
    ((volatile u16*)__cpReg)[2] = 4;
}
