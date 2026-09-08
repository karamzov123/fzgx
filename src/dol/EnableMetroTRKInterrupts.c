#include "types.h"

extern u32 fn_8008D440(void);

void EnableMetroTRKInterrupts(void) {
    fn_8008D440();
}
