#include "types.h"

extern void fn_80008BEC(void *dst, s32 value, u32 size);
extern void fn_80008BA8(void *dst, const void *src, u32 size);

void fn_1_80058(s8 arg0, u8 *arg1) {
    u8 data[0x20];

    fn_80008BEC(data, 0, 0x20);
    *(u32 *)(data + 4) = 0xF;
    data[8] = 1;
    fn_80008BA8(arg1, data, 0x20);
    *(u16 *)(arg1 + 2) = arg0;
}
