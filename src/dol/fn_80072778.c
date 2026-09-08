#include "types.h"

extern u32 fn_80072A50(u32, u32, u32, u32, u32);
extern u32 lbl_801A6D38;
extern u32 memset(u32, u32, u32);

s32 fn_80072778(void) {
    memset(lbl_801A6D38, 255, 3232);
    fn_80072A50(0, 0, 1, 2, 3);
    fn_80072A50(1, 0, 1, 2, 0);
    fn_80072A50(2, 0, 1, 2, 1);
    fn_80072A50(3, 0, 1, 2, 2);
    return 1;
}
