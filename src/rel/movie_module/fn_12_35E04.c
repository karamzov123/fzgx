#include "types.h"

extern u32 fn_12_33BC(u32, u32);
extern u32 fn_12_341C(void);
extern void fn_12_35DDC(void);

u32 fn_12_35E04(void) {
    fn_12_341C();
    return fn_12_33BC((u32)fn_12_35DDC, 0);
}
