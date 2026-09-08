#include "types.h"

extern u32 PPCHalt(void);

u32 _ExitProcess(void) {
    return PPCHalt();
}
