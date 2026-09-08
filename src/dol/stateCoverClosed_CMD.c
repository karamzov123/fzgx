#include "types.h"

extern u32 fn_8001687C(u32, u32);
extern u32 lbl_8015CE60[];
extern void fn_80018650(void);

void stateCoverClosed_CMD(void) {
    fn_8001687C((u32)&lbl_8015CE60, (u32)fn_80018650);
}
