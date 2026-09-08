#include "types.h"

extern void fn_80006B4C(void);
extern u32 lbl_801A6708[2];

void fn_80006B30(void (*func)(void)) {
    lbl_801A6708[0] = (u32)(func != 0 ? func : fn_80006B4C);
}
