#include "types.h"


u32 AIGetStreamPlayState(void) {
    return (*(u32 *)0xCC006C00 & 0x1);  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
}
