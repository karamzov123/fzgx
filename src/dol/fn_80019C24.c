#include "types.h"

extern u32 __DVDThreadQueue;
extern u32 fn_80011194(u32);

void fn_80019C24(void) {
    fn_80011194((u32)&__DVDThreadQueue);
}
