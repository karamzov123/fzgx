#include "types.h"


u32 AIGetStreamVolRight(void) {
    return ((*(u32 *)0xCC006C04 >> 8) & 0xFF);  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
}
