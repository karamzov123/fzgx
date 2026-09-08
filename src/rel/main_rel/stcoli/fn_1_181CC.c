#include "types.h"

// fn_1_181CC: main_rel .text:0x000181CC size 0x24
// Wrapper function that passes first three arguments through and sets fourth to 1

extern void fn_1_180F4(int, int, int, int);

void fn_1_181CC(int a, int b, int c) {
    fn_1_180F4(a, b, c, 1);
}
