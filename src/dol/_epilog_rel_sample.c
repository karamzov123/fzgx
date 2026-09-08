#include "types.h"

extern u32 fn_800169AC(u32);
extern void fn_80017B14(void);

u32 _epilog_rel_sample(void) {
    return fn_800169AC((u32)fn_80017B14);
}
