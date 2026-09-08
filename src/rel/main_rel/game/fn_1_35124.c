#include "types.h"

extern u32 OSGetTick(void);
extern void fn_1_A0978(void);

void fn_1_35124(void) {
    OSGetTick();
    fn_1_A0978();
    OSGetTick();
}
