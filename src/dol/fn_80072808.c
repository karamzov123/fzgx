#include "types.h"

extern u32 fn_80072A50(u32, u32, u32, u32, u32);
extern u32 lbl_801A6D38;
extern u32 memset(u32, u32, u32);

void fn_80072808(void) {
    memset(lbl_801A6D38, 255, 3232);
    fn_80072A50(0, 0, 1, 2, 3);
    fn_80072A50(1, 0, 1, 2, 0);
}
