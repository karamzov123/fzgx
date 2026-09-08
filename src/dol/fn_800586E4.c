#include "types.h"

extern u32 fn_80058A20(u32);
extern u32 lbl_80092340[];

void fn_800586E4(void) {
    fn_80058A20((u32)&lbl_80092340);
}
