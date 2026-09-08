#include "types.h"

extern void fn_1_7ECB8(u32 arg0, u32 arg1, u32 arg2);

void fn_1_7F1E8(u32 arg0, u32 arg1) {
    fn_1_7ECB8(arg0, arg1, 0x80000000); // fzgx-allow: A1 retail sentinel
}
