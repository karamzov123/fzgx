#include "types.h"

extern u32 OSGetResetCode(void);
extern int __cntlzw(unsigned int value);
extern void fn_800057F8(void);
extern u32 lbl_801A66B4;
extern void fn_80005660(void);
extern void fn_8000591C(void);

void main(int argc, char **argv) {
    u32 reset_code;
    u32 reset_match;
    u32 *base;
    u32 value;

    reset_code = OSGetResetCode();
    reset_match = (u32)__cntlzw(((u32)1 << 31) - reset_code) >> 5;
    if (reset_match) {
        fn_800057F8();
    }

    value = 0x4C000064;
    base = (u32 *)((u32)0x40000000 << 1);
    base[0x200 / 4] = value;
    base[0x300 / 4] = value;
    base[0x400 / 4] = value;
    base[0x600 / 4] = value;
    base[0x700 / 4] = value;
    base[0xC00 / 4] = value;
    base[0xD00 / 4] = value;
    base[0x1000 / 4] = value;
    base[0x1100 / 4] = value;
    base[0x1200 / 4] = value;
    base[0x1300 / 4] = value;
    lbl_801A66B4 = 0;
    fn_80005660();
    fn_8000591C();
}
