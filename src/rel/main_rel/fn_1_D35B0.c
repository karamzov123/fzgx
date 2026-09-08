#include "types.h"

extern void fn_80071100(void);
extern void fn_1_D3B6C(void);

// Wrapper that calls fn_80071100 followed by fn_1_D3B6C.
void fn_1_D35B0(void) {
    fn_80071100();
    fn_1_D3B6C();
}
