#include "types.h"

s32 OSIsThreadTerminated(void* thread) {
    u16 state = *(u16*)((u8*)thread + 0x2C8);
    return (state == 8 || state == 0) ? 1 : 0;
}
