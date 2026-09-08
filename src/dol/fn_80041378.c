#include "types.h"

extern u32 fn_80047608(void);
extern u32 fn_8004BDD8(void);
extern u32 fn_80056C64(void);

s32 fn_80041378(void) {
    fn_8004BDD8();
    fn_80047608();
    fn_80056C64();
    return 0;
}
