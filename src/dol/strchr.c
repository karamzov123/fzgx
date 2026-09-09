#include "types.h"

char* strchr(const char* str, s32 chr) {
    const u8* p = (const u8*)str - 1;
    u32 c = (u32)chr & 0xFF;
    u32 ch;
    while (ch = *++p) {
        if (ch == c) {
            return (char*)p;
        }
    }
    return c ? 0 : (char*)p;
}
