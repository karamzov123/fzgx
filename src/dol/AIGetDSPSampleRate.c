#include "types.h"


u32 AIGetDSPSampleRate(void) {
    return (((*(u32 *)0xCC006C00 >> 6) & 0x1) ^ 1);  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
}
