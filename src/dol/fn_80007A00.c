#include "types.h"

extern u32 fn_80007654(u32 value);

u32 fn_80007A00(u32 value) {
    s32 result;

    result = fn_80007654(value);
    result = ((((value >> 12) & 0xffff) ^ (result & 0x7ffff)) << 6);
    return (result ^ 0x01ff0000) ^ 0xffc0;
}
