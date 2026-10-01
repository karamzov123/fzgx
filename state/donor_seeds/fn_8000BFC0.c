#include "types.h"

// Donor seed for fn_8000BFC0 (addr 0x8000BFC0)
// Extracted from OSContext.c (original name: OSGetStackPointer)

u32 fn_8000BFC0(void)
{
    register u32 sp;

    asm
    {
    mr      sp, r1
    }
    return sp;
}
