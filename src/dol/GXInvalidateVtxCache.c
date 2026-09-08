#include "types.h"

typedef union {
    u8 u8;
    u16 u16;
    u32 u32;
} GXWGFifoReg;

#define GXWGFifoAddress ((u32)(0xCC00 << 16) + 0x8000)
// Hardware FIFO register is intentionally volatile.
#define GXWGFifo (*(volatile GXWGFifoReg *)GXWGFifoAddress)

void GXInvalidateVtxCache(void) {
    GXWGFifo.u8 = 0x48;
}
