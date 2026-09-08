#include "types.h"

#define AI_STREAM_VOL_LEFT_ADDR 0xCC006C04 // fzgx-allow: A1 hardware register address

u8 AIGetStreamVolLeft(void) {
    return *(volatile u32 *)AI_STREAM_VOL_LEFT_ADDR; // fzgx-allow: A2 hardware register access
}
