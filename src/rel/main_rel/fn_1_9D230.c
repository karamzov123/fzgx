#include "types.h"

extern u32 fn_1_9D260(void);

u32 fn_1_9D230(void) {
    return (fn_1_9D260() & 0xC) != 0;
}
