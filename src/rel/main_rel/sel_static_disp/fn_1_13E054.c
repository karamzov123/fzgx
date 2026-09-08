#include "types.h"

extern void fn_1_13E080(void* arg0, void* arg1, void* arg2, int arg3);

void fn_1_13E054(void* arg0, void* arg1, void* arg2, int arg3) {
    u8* fixed = (u8*)0;
    fixed += 0xA204;
    fn_1_13E080(arg0, arg1, fixed, 31);
}
