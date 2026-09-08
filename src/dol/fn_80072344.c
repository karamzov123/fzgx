#include "types.h"

extern u32 lbl_8019F008[];
extern u32 lbl_8019F024[];
extern u32 memcpy(u32, u32, u32);

void fn_80072344(void) {
    memcpy((u32)&lbl_8019F024, (u32)&lbl_8019F008, 28);
}
