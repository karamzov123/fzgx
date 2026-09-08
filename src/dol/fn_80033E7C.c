#include "types.h"

extern u32 fn_8000A0B8(void);
extern void PPCMtwpar(u32);

void fn_80033E7C(void) {
    u32 status;

    do {
        status = fn_8000A0B8();
    } while ((status & 1u) != 0);

    PPCMtwpar(0x0C008000); // fzgx-allow: A1 fixed PPC hardware address
}
