#include "types.h"

extern u32 __GXFlushTextureState(void);

void GXInvalidateTexAll(void) {
    u32 v0;
    __GXFlushTextureState();
    *(u8 *)((u8 *)0xCC010000 + -32768) = 97;  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
    v0 = (0x66000000 + 4096);
    *(u32 *)((u8 *)0xCC010000 + -32768) = v0;  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
    *(u8 *)((u8 *)0xCC010000 + -32768) = 97;  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
    *(u32 *)((u8 *)0xCC010000 + -32768) = (0x66000000 + 4352);  /* fzgx-allow: A1,A2 unnamed OS/hardware memory */
    __GXFlushTextureState();
}
