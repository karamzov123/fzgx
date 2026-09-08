#include "types.h"

void fn_1_B03A8(void *arg0, void *arg1) {
    void *child = *(void **)((u8 *)arg1 + 0x24);
    if ((*(u32 *)((u8 *)child + 0x8) & 0x20) == 0) {
        *(u32 *)((u8 *)child + 0x90) = *(u32 *)((u8 *)arg0 + 0xc);
    }
    *(u8 *)((u8 *)arg1 + 0x3) = 0x3;
}
