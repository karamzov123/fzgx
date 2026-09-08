#include "types.h"


u32 DSPCheckMailFromDSP(void) {
    return ((*(u16 *)0xCC005004 >> 15) & 0x1);  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
}
