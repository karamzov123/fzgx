#include "types.h"

// fn_1_7EAE8: main_rel .text:0x0007EAE8 size 0x24
// Wrapper that calls fn_1_5634C with argument 0.

extern void fn_1_5634C(u32 arg0);

void fn_1_7EAE8(void) {
    fn_1_5634C(0);
}
