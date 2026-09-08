#include "types.h"

extern u32 __stdio_exit;
extern void __close_all(void);

void __stdio_atexit(void) {
    __stdio_exit = (u32)__close_all;
}
