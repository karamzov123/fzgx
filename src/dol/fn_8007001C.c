#include "types.h"

extern u32 fn_80037518(u32, u32, u32, u32, u32);
extern u32 fn_80072BD0(u32);
extern u32 fn_80074918(u32, u32, u32);

void fn_8007001C(void) {
    fn_80074918(1, 3, 1);
    fn_80037518(7, 0, 0, 7, 0);
    fn_80072BD0(1);
}
