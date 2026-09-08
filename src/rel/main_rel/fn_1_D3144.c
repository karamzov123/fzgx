#include "types.h"

extern u32 lbl_1_bss_7AD00;

// fn_1_D3144: Conditionally call function pointer from BSS
void fn_1_D3144(void) {
    u32 fn_ptr = lbl_1_bss_7AD00;
    if (fn_ptr != 0) {
        ((void (*)(void))fn_ptr)();
    }
}
